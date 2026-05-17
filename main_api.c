#include "include/bst.h"
#include "include/linked_list.h"
#include "include/recursion.h"
#include "include/stack.h"

#include <fcntl.h>
#include <stdarg.h>
#include <sys/types.h>
#include <unistd.h>

#define HISTORY_FILE "data/history.txt"
#define EVENTS_FILE "data/events.txt"
#define JSON_BUFFER_SIZE 262144
#define CAPTURE_BUFFER_SIZE 131072

static TList *g_personality_list = NULL;
static TList *g_date_list = NULL;
static TList *g_event_list = NULL;
static TStack *g_stack = NULL;
static TStack *g_event_stack = NULL;
static TTree *g_tree = NULL;
static int g_loaded = 0;

static char g_json[JSON_BUFFER_SIZE];
static size_t g_json_len = 0;
static char g_capture[CAPTURE_BUFFER_SIZE];
static size_t g_capture_len = 0;

static void free_stack_nodes(TStack *head) {
    while (head != NULL) {
        TStack *next = head->next;
        free(head);
        head = next;
    }
}

static void free_tree_nodes(TTree *root) {
    if (root == NULL) {
        return;
    }
    free_tree_nodes(root->left);
    free_tree_nodes(root->right);
    free(root);
}

static void json_reset(void) {
    g_json[0] = '\0';
    g_json_len = 0;
}

static void json_append_raw(const char *text) {
    size_t available;
    size_t length;

    if (text == NULL || g_json_len >= JSON_BUFFER_SIZE - 1) {
        return;
    }

    available = JSON_BUFFER_SIZE - g_json_len - 1;
    length = strlen(text);
    if (length > available) {
        length = available;
    }

    memcpy(g_json + g_json_len, text, length);
    g_json_len += length;
    g_json[g_json_len] = '\0';
}

static void json_appendf(const char *fmt, ...) {
    char buffer[2048];
    va_list args;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    json_append_raw(buffer);
}

static void json_append_escaped(const char *text) {
    const unsigned char *cursor = (const unsigned char *)(text != NULL ? text : "");

    while (*cursor != '\0' && g_json_len < JSON_BUFFER_SIZE - 1) {
        switch (*cursor) {
            case '\\':
                json_append_raw("\\\\");
                break;
            case '"':
                json_append_raw("\\\"");
                break;
            case '\n':
                json_append_raw("\\n");
                break;
            case '\r':
                json_append_raw("\\r");
                break;
            case '\t':
                json_append_raw("\\t");
                break;
            default:
                if (*cursor < 32) {
                    json_appendf("\\u%04x", (unsigned int)*cursor);
                } else {
                    char one[2];
                    one[0] = (char)*cursor;
                    one[1] = '\0';
                    json_append_raw(one);
                }
                break;
        }
        cursor++;
    }
}

static const char *json_error(const char *message) {
    json_reset();
    json_append_raw("{\"status\":\"error\",\"message\":\"");
    json_append_escaped(message != NULL ? message : "Unknown error");
    json_append_raw("\"}");
    return g_json;
}

static const char *json_ok_output(const char *output) {
    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":{\"output\":\"");
    json_append_escaped(output != NULL ? output : "");
    json_append_raw("\"}}");
    return g_json;
}

static const char *json_ok_int(const char *key, int value) {
    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":{\"");
    json_append_raw(key);
    json_append_raw("\":");
    json_appendf("%d", value);
    json_append_raw("}}");
    return g_json;
}

static const char *json_ok_bool(const char *key, int value) {
    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":{\"");
    json_append_raw(key);
    json_append_raw("\":");
    json_append_raw(value ? "true" : "false");
    json_append_raw("}}");
    return g_json;
}

static void json_append_date_object(const date *value) {
    if (value == NULL) {
        json_append_raw("null");
        return;
    }

    json_append_raw("{\"day\":");
    json_appendf("%d", value->day);
    json_append_raw(",\"month\":");
    json_appendf("%d", value->month);
    json_append_raw(",\"year\":");
    json_appendf("%d", value->year);
    json_append_raw("}");
}

static void json_append_list_object(const TList *node) {
    json_append_raw("{\"name\":\"");
    json_append_escaped(node->name);
    json_append_raw("\",\"definition\":\"");
    json_append_escaped(node->definition);
    json_append_raw("\",\"dob\":\"");
    json_append_escaped(node->DoB);
    json_append_raw("\",\"dod\":\"");
    json_append_escaped(node->DoD);
    json_append_raw("\"}");
}

static void json_append_stack_object(const TStack *node) {
    json_append_raw("{\"name\":\"");
    json_append_escaped(node->name);
    json_append_raw("\",\"definition\":\"");
    json_append_escaped(node->definition);
    json_append_raw("\",\"dob\":\"");
    json_append_escaped(node->DoB);
    json_append_raw("\",\"dod\":\"");
    json_append_escaped(node->DoD);
    json_append_raw("\",\"event_date\":\"");
    json_append_escaped(node->event_date);
    json_append_raw("\"}");
}

static void json_append_tree_object(const TTree *node) {
    json_append_raw("{\"name\":\"");
    json_append_escaped(node->name);
    json_append_raw("\",\"definition\":\"");
    json_append_escaped(node->definition);
    json_append_raw("\",\"dob\":\"");
    json_append_escaped(node->DoB);
    json_append_raw("\",\"dod\":\"");
    json_append_escaped(node->DoD);
    json_append_raw("\",\"event_date\":\"");
    json_append_escaped(node->event_date);
    json_append_raw("\"}");
}

static const char *json_ok_date(const date *value) {
    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":");
    json_append_date_object(value);
    json_append_raw("}");
    return g_json;
}

static const char *json_ok_list(TList *head) {
    TList *current = head;
    int first = 1;

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":[");
    while (current != NULL) {
        if (!first) {
            json_append_raw(",");
        }
        json_append_list_object(current);
        first = 0;
        current = current->next;
    }
    json_append_raw("]}");
    return g_json;
}

static const char *json_ok_circular_list(TList *head, int max_nodes) {
    TList *current = head;
    int count = 0;
    int first = 1;

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":[");
    while (current != NULL && count < max_nodes) {
        if (!first) {
            json_append_raw(",");
        }
        json_append_list_object(current);
        first = 0;
        count++;
        current = current->next;
        if (current == head) {
            break;
        }
    }
    json_append_raw("]}");
    return g_json;
}

static const char *json_ok_stack(TStack *head) {
    TStack *current = head;
    int first = 1;

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":[");
    while (current != NULL) {
        if (!first) {
            json_append_raw(",");
        }
        json_append_stack_object(current);
        first = 0;
        current = current->next;
    }
    json_append_raw("]}");
    return g_json;
}

static void json_append_tree_in_order(TTree *root, int *first) {
    if (root == NULL) {
        return;
    }

    json_append_tree_in_order(root->left, first);
    if (!*first) {
        json_append_raw(",");
    }
    json_append_tree_object(root);
    *first = 0;
    json_append_tree_in_order(root->right, first);
}

static const char *json_ok_tree(TTree *root) {
    int first = 1;

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":[");
    json_append_tree_in_order(root, &first);
    json_append_raw("]}");
    return g_json;
}

static const char *json_ok_tree_node(TTree *node) {
    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":");
    json_append_tree_object(node);
    json_append_raw("}");
    return g_json;
}

static const char *json_ok_stack_node(TStack *node) {
    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":");
    json_append_stack_object(node);
    json_append_raw("}");
    return g_json;
}

static const char *json_ok_pronunciation(TStack *short_stack, TStack *long_stack) {
    TStack *current;
    int first;

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":{\"short_stack\":[");
    current = short_stack;
    first = 1;
    while (current != NULL) {
        if (!first) {
            json_append_raw(",");
        }
        json_append_stack_object(current);
        first = 0;
        current = current->next;
    }

    json_append_raw("],\"long_stack\":[");
    current = long_stack;
    first = 1;
    while (current != NULL) {
        if (!first) {
            json_append_raw(",");
        }
        json_append_stack_object(current);
        first = 0;
        current = current->next;
    }
    json_append_raw("]}}");
    return g_json;
}

static int parse_date_string(const char *text, date *out) {
    if (text == NULL || out == NULL || text[0] == '\0') {
        return 0;
    }

    memset(out, 0, sizeof(*out));
    if (sscanf(text, "%d/%d/%d", &out->day, &out->month, &out->year) != 3) {
        return 0;
    }
    strncpy(out->full_date, text, sizeof(out->full_date) - 1);
    out->full_date[sizeof(out->full_date) - 1] = '\0';
    return 1;
}

static int count_list_nodes(TList *head) {
    int count = 0;

    while (head != NULL) {
        count++;
        head = head->next;
    }
    return count;
}

static void load_event_views(void) {
    FILE *file = fopen(EVENTS_FILE, "r");
    char line[1024];

    if (g_event_list != NULL) {
        freeList(g_event_list);
        g_event_list = NULL;
    }
    if (g_event_stack != NULL) {
        free_stack_nodes(g_event_stack);
        g_event_stack = NULL;
    }

    if (file == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *eq;
        char *open_brace;
        char *close_brace;
        char event_name[100];
        char event_date[20];
        TStack *new_node;

        line[strcspn(line, "\n")] = '\0';
        eq = strchr(line, '=');
        if (eq == NULL) {
            continue;
        }

        *eq = '\0';
        open_brace = strchr(eq + 1, '{');
        if (open_brace == NULL) {
            continue;
        }

        close_brace = strchr(open_brace + 1, '}');
        if (close_brace == NULL) {
            continue;
        }

        *close_brace = '\0';

        strncpy(event_name, line, sizeof(event_name) - 1);
        event_name[sizeof(event_name) - 1] = '\0';
        strncpy(event_date, open_brace + 1, sizeof(event_date) - 1);
        event_date[sizeof(event_date) - 1] = '\0';

        g_event_list = insertAtTail(g_event_list, event_name, "", "", event_date);

        new_node = (TStack *)malloc(sizeof(TStack));
        if (new_node == NULL) {
            continue;
        }
        strncpy(new_node->name, event_name, sizeof(new_node->name) - 1);
        new_node->name[sizeof(new_node->name) - 1] = '\0';
        new_node->definition[0] = '\0';
        new_node->DoB[0] = '\0';
        new_node->DoD[0] = '\0';
        strncpy(new_node->event_date, event_date, sizeof(new_node->event_date) - 1);
        new_node->event_date[sizeof(new_node->event_date) - 1] = '\0';
        new_node->next = g_event_stack;
        g_event_stack = new_node;
    }

    fclose(file);
}

static void ensure_loaded(void) {
    FILE *file;
    char line[1024];

    if (g_loaded) {
        return;
    }

    file = fopen(HISTORY_FILE, "r");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file) != NULL) {
            char *eq;
            char *definition_start;
            char *first_open;
            char *first_close;
            char *second_open;
            char *second_close;
            char definition[500] = "";
            char dob[20] = "";
            char dod[20] = "";

            line[strcspn(line, "\n")] = '\0';
            eq = strchr(line, '=');
            if (eq == NULL) {
                continue;
            }

            *eq = '\0';
            definition_start = eq + 1;
            first_open = strchr(definition_start, '{');
            if (first_open == NULL) {
                continue;
            }

            {
                size_t definition_length = (size_t)(first_open - definition_start);
                if (definition_length >= sizeof(definition)) {
                    definition_length = sizeof(definition) - 1;
                }
                memcpy(definition, definition_start, definition_length);
                definition[definition_length] = '\0';
            }

            first_close = strchr(first_open + 1, '}');
            if (first_close != NULL) {
                size_t dob_length = (size_t)(first_close - (first_open + 1));
                if (dob_length >= sizeof(dob)) {
                    dob_length = sizeof(dob) - 1;
                }
                memcpy(dob, first_open + 1, dob_length);
                dob[dob_length] = '\0';

                second_open = strchr(first_close + 1, '{');
                if (second_open != NULL) {
                    second_close = strchr(second_open + 1, '}');
                    if (second_close != NULL) {
                        size_t dod_length = (size_t)(second_close - (second_open + 1));
                        if (dod_length >= sizeof(dod)) {
                            dod_length = sizeof(dod) - 1;
                        }
                        memcpy(dod, second_open + 1, dod_length);
                        dod[dod_length] = '\0';
                    }
                }
            }

            g_personality_list = insertAtTail(g_personality_list, line, definition, dob, dod);
            g_date_list = insertAtTail(g_date_list, line, definition, dob, dod);
        }
        fclose(file);
    }

    g_stack = toStack(g_personality_list);
    g_tree = toTree(g_stack);
    load_event_views();
    g_loaded = 1;
}

static TStack *active_event_stack(void) {
    return g_event_stack != NULL ? g_event_stack : g_stack;
}

static int capture_begin(int pipefd[2], int *saved_stdout) {
    if (pipe(pipefd) == -1) {
        return 0;
    }

    *saved_stdout = dup(STDOUT_FILENO);
    if (*saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        pipefd[0] = -1;
        pipefd[1] = -1;
        return 0;
    }

    fflush(stdout);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);
    pipefd[1] = -1;
    return 1;
}

static void capture_end(int pipefd[2], int saved_stdout) {
    ssize_t read_count;
    char chunk[1024];

    g_capture[0] = '\0';
    g_capture_len = 0;

    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    while ((read_count = read(pipefd[0], chunk, sizeof(chunk) - 1)) > 0) {
        size_t available = CAPTURE_BUFFER_SIZE - g_capture_len - 1;
        size_t length = (size_t)read_count;
        if (length > available) {
            length = available;
        }
        memcpy(g_capture + g_capture_len, chunk, length);
        g_capture_len += length;
        if (g_capture_len >= CAPTURE_BUFFER_SIZE - 1) {
            break;
        }
    }

    g_capture[g_capture_len] = '\0';
    close(pipefd[0]);
    pipefd[0] = -1;
}

const char *api_insertToBST(const char *name, const char *definition, const char *dob,
                            const char *dod, const char *event_date) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_tree = insertToBST(g_tree,
                         (char *)name,
                         (char *)(definition != NULL ? definition : ""),
                         (char *)(dob != NULL ? dob : ""),
                         (char *)(dod != NULL ? dod : ""),
                         (char *)(event_date != NULL ? event_date : ""));
    if (g_tree == NULL) {
        return json_error("insertToBST returned NULL");
    }
    return json_ok_tree(g_tree);
}

const char *api_fillTree(const char *csv_text) {
    FILE *file;
    TTree *new_tree;

    if (csv_text == NULL || csv_text[0] == '\0') {
        return json_error("csv_text is required");
    }

    file = tmpfile();
    if (file == NULL) {
        return json_error("failed to create temporary file for fillTree");
    }

    fputs(csv_text, file);
    rewind(file);
    new_tree = fillTree(file);
    fclose(file);

    free_tree_nodes(g_tree);
    g_tree = new_tree;
    g_loaded = 1;
    return json_ok_tree(g_tree);
}

const char *api_getInfoNameTree(const char *name) {
    TTree *result;

    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    result = getInfoNameTree(g_tree, (char *)name);
    if (result == NULL) {
        return json_error("name not found in tree");
    }
    return json_ok_tree_node(result);
}

const char *api_addNameBST(const char *name, const char *dob, const char *dod) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_tree = addNameBST(g_tree,
                        (char *)name,
                        (char *)(dob != NULL ? dob : ""),
                        (char *)(dod != NULL ? dod : ""));
    return json_ok_tree(g_tree);
}

const char *api_deleteNameBST(const char *name) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_tree = deleteNameBST(g_tree, (char *)name);
    return json_ok_tree(g_tree);
}

const char *api_updateNameBST(const char *name, const char *definition, const char *dob,
                              const char *dod) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_tree = updateNameBST(g_tree,
                           (char *)name,
                           (char *)(definition != NULL ? definition : ""),
                           (char *)(dob != NULL ? dob : ""),
                           (char *)(dod != NULL ? dod : ""));
    return json_ok_tree(g_tree);
}

const char *api_traversalBSTinOrder(void) {
    TTree *result;
    const char *json;

    ensure_loaded();
    result = traversalBSTinOrder(g_tree);
    json = json_ok_tree(result);
    free_tree_nodes(result);
    return json;
}

const char *api_getPersonality(void) {
    ensure_loaded();
    return json_ok_list(g_personality_list);
}

const char *api_getDatePersonality(void) {
    ensure_loaded();
    return json_ok_list(g_date_list);
}

const char *api_getInfoByDates(const char *dob) {
    int pipefd[2] = {-1, -1};
    int saved_stdout;

    ensure_loaded();
    if (dob == NULL || dob[0] == '\0') {
        return json_error("dob is required");
    }
    if (!capture_begin(pipefd, &saved_stdout)) {
        return json_error("failed to capture stdout");
    }
    getInfoByDates(g_date_list, (char *)dob);
    capture_end(pipefd, saved_stdout);
    return json_ok_output(g_capture);
}

const char *api_getInfoByDates2(const char *dod) {
    int pipefd[2] = {-1, -1};
    int saved_stdout;

    ensure_loaded();
    if (dod == NULL || dod[0] == '\0') {
        return json_error("dod is required");
    }
    if (!capture_begin(pipefd, &saved_stdout)) {
        return json_error("failed to capture stdout");
    }
    getInfoByDates2(g_date_list, (char *)dod);
    capture_end(pipefd, saved_stdout);
    return json_ok_output(g_capture);
}

const char *api_sortWord(void) {
    ensure_loaded();
    g_personality_list = sortWord(g_personality_list);
    return json_ok_list(g_personality_list);
}

const char *api_sortWord2(void) {
    ensure_loaded();
    g_personality_list = sortWord2(g_personality_list);
    return json_ok_list(g_personality_list);
}

const char *api_convert(const char *date_text) {
    date *value;
    const char *json;

    if (date_text == NULL || date_text[0] == '\0') {
        return json_error("date_text is required");
    }

    value = convert((char *)date_text);
    if (value == NULL) {
        return json_error("convert failed");
    }
    json = json_ok_date(value);
    free(value);
    return json;
}

const char *api_agecalculate(const char *start_date, const char *end_date) {
    date *start;
    date *end;
    int age;

    if (start_date == NULL || start_date[0] == '\0') {
        return json_error("start_date is required");
    }

    start = convert((char *)start_date);
    end = (end_date != NULL && end_date[0] != '\0') ? convert((char *)end_date) : NULL;
    age = agecalculate(start, end);

    if (start != NULL) {
        free(start);
    }
    if (end != NULL) {
        free(end);
    }

    return json_ok_int("age", age);
}

const char *api_sortPersonality(void) {
    ensure_loaded();
    g_date_list = sortPersonality(g_date_list);
    return json_ok_list(g_date_list);
}

const char *api_deletepersonality(const char *name) {
    FILE *file;

    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return json_error("failed to open history file");
    }

    g_personality_list = deletepersonality(file, g_personality_list, g_date_list, (char *)name);
    return json_ok_list(g_personality_list);
}

const char *api_updatePersonality(const char *name, const char *definition, const char *dob,
                                  const char *dod) {
    FILE *file;

    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return json_error("failed to open history file");
    }

    g_personality_list = updatePersonality(file,
                                           g_personality_list,
                                           g_date_list,
                                           (char *)name,
                                           (char *)(definition != NULL ? definition : ""),
                                           (char *)(dob != NULL ? dob : ""),
                                           (char *)(dod != NULL ? dod : ""));
    return json_ok_list(g_personality_list);
}

const char *api_similarPersonality(const char *word) {
    TList *result;
    const char *json;

    ensure_loaded();
    if (word == NULL || word[0] == '\0') {
        return json_error("word is required");
    }

    result = similarPersonality(g_date_list, (char *)word);
    json = json_ok_list(result);
    if (result != NULL) {
        freeList(result);
    }
    return json;
}

const char *api_countPersonality(const char *date_text) {
    TList *result;
    date value;
    const char *json;

    ensure_loaded();
    if (!parse_date_string(date_text, &value)) {
        return json_error("date_text must use DD/MM/YYYY");
    }

    result = countPersonality(g_date_list, &value);
    json = json_ok_list(result);
    if (result != NULL) {
        freeList(result);
    }
    return json;
}

const char *api_isPalindrome(const char *word) {
    if (word == NULL || word[0] == '\0') {
        return json_error("word is required");
    }
    return json_ok_bool("result", isPalindrome((char *)word));
}

const char *api_palindromeName(void) {
    TList *result;
    const char *json;

    ensure_loaded();
    result = palindromeName(g_personality_list);
    json = json_ok_list(result);
    if (result != NULL) {
        freeList(result);
    }
    return json;
}

const char *api_mergeNodes(void) {
    TList *result;
    const char *json;

    ensure_loaded();
    result = mergeNodes(g_personality_list, g_date_list);
    json = json_ok_list(result);
    if (result != NULL) {
        freeList(result);
    }
    return json;
}

const char *api_merge2Nodes(void) {
    TList *result;
    const char *json;
    int max_nodes;

    ensure_loaded();
    result = merge2Nodes(g_personality_list, g_date_list);
    max_nodes = count_list_nodes(g_personality_list);
    if (count_list_nodes(g_date_list) < max_nodes) {
        max_nodes = count_list_nodes(g_date_list);
    }

    json = json_ok_circular_list(result, max_nodes);
    if (result != NULL) {
        TList *tail = result;
        while (tail->next != result) {
            tail = tail->next;
        }
        tail->next = NULL;
        freeList(result);
    }
    return json;
}

const char *api_addPersonality(const char *name, const char *definition, const char *dob,
                               const char *dod) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_personality_list = addPersonality(g_personality_list,
                                        g_date_list,
                                        (char *)name,
                                        (char *)(definition != NULL ? definition : ""),
                                        (char *)(dob != NULL ? dob : ""),
                                        (char *)(dod != NULL ? dod : ""));
    g_date_list = insertAtHead(g_date_list,
                               (char *)name,
                               (char *)(definition != NULL ? definition : ""),
                               (char *)(dob != NULL ? dob : ""),
                               (char *)(dod != NULL ? dod : ""));
    return json_ok_list(g_personality_list);
}

const char *api_addEvents(const char *name, const char *date_text) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }
    if (date_text == NULL || date_text[0] == '\0') {
        return json_error("date is required");
    }

    g_event_list = addEvents(g_event_list, (char *)name, (char *)date_text);
    load_event_views();
    return json_ok_list(g_event_list);
}

const char *api_countOccurence(const char *name) {
    FILE *file;
    int count;

    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return json_error("failed to open history file");
    }

    count = countOccurence(file, (char *)name);
    fclose(file);
    return json_ok_int("count", count);
}

static const char *json_ok_file_content(FILE *file) {
    char buffer[1024];

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":{\"file_content\":\"");
    rewind(file);
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        json_append_escaped(buffer);
    }
    json_append_raw("\"}}");
    return g_json;
}

const char *api_removeOccurrence(const char *word) {
    FILE *file;
    FILE *tmp;
    const char *json;

    if (word == NULL || word[0] == '\0') {
        return json_error("word is required");
    }

    file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return json_error("failed to open history file");
    }

    tmp = tmpfile();
    if (tmp == NULL) {
        fclose(file);
        return json_error("failed to create temporary file");
    }

    removeOccurrence(file, tmp, (char *)word);
    json = json_ok_file_content(tmp);
    fclose(file);
    fclose(tmp);
    return json;
}

const char *api_replaceOccurence(const char *name, const char *definition, const char *dob,
                                 const char *dod) {
    FILE *file;
    FILE *tmp;
    const char *json;

    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    file = fopen(HISTORY_FILE, "r");
    if (file == NULL) {
        return json_error("failed to open history file");
    }

    tmp = tmpfile();
    if (tmp == NULL) {
        fclose(file);
        return json_error("failed to create temporary file");
    }

    replaceOccurence(file,
                     tmp,
                     (char *)name,
                     (char *)(definition != NULL ? definition : ""),
                     (char *)(dob != NULL ? dob : ""),
                     (char *)(dod != NULL ? dod : ""));
    json = json_ok_file_content(tmp);
    fclose(file);
    fclose(tmp);
    return json;
}

const char *api_namePermutation(const char *name) {
    int pipefd[2] = {-1, -1};
    int saved_stdout;
    char buffer[128];
    int length;

    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    length = (int)strlen(name);
    if (length > 8) {
        return json_error("namePermutation is limited to 8 characters for safe capture");
    }

    strncpy(buffer, name, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (!capture_begin(pipefd, &saved_stdout)) {
        return json_error("failed to capture stdout");
    }
    namePermutation(buffer, 0, length - 1);
    capture_end(pipefd, saved_stdout);
    return json_ok_output(g_capture);
}

const char *api_printSub(const char *word, const char *res, int i, int j) {
    int pipefd[2] = {-1, -1};
    int saved_stdout;
    char word_buffer[256];
    char res_buffer[256];

    if (word == NULL || word[0] == '\0') {
        return json_error("word is required");
    }
    if ((int)strlen(word) > 12) {
        return json_error("printSub is limited to 12 characters for safe capture");
    }
    if (i < 0 || j < 0 || i > (int)strlen(word) || j > (int)strlen(word)) {
        return json_error("i and j must be within the word bounds");
    }

    strncpy(word_buffer, word, sizeof(word_buffer) - 1);
    word_buffer[sizeof(word_buffer) - 1] = '\0';
    strncpy(res_buffer, res != NULL ? res : "", sizeof(res_buffer) - 1);
    res_buffer[sizeof(res_buffer) - 1] = '\0';

    if (!capture_begin(pipefd, &saved_stdout)) {
        return json_error("failed to capture stdout");
    }
    printSub(word_buffer, res_buffer, i, j);
    capture_end(pipefd, saved_stdout);
    return json_ok_output(g_capture);
}

const char *api_subseqName(const char *word) {
    int pipefd[2] = {-1, -1};
    int saved_stdout;
    char buffer[256];

    if (word == NULL || word[0] == '\0') {
        return json_error("word is required");
    }
    if ((int)strlen(word) > 12) {
        return json_error("subseqName is limited to 12 characters for safe capture");
    }

    strncpy(buffer, word, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (!capture_begin(pipefd, &saved_stdout)) {
        return json_error("failed to capture stdout");
    }
    subseqName(buffer);
    capture_end(pipefd, saved_stdout);
    return json_ok_output(g_capture);
}

const char *api_distinctSubseqWord(const char *event) {
    if (event == NULL || event[0] == '\0') {
        return json_error("event is required");
    }
    return json_ok_int("count", distinctSubseqWord((char *)event));
}

const char *api_isPalindromeWord(const char *event, int i, int j) {
    int length;

    if (event == NULL) {
        return json_error("event is required");
    }
    length = (int)strlen(event);
    if (length == 0) {
        return json_ok_bool("result", 1);
    }
    if (i < 0 || j < 0 || i >= length || j >= length) {
        return json_error("i and j must be within the event bounds");
    }
    return json_ok_bool("result", isPalindromeWord((char *)event, i, j));
}

const char *api_longestSubyear(const char *date1, const char *date2) {
    FILE *file;
    int pipefd[2] = {-1, -1};
    int saved_stdout;

    if (date1 == NULL || date1[0] == '\0' || date2 == NULL || date2[0] == '\0') {
        return json_error("date1 and date2 are required");
    }

    file = fopen(EVENTS_FILE, "r");
    if (file == NULL) {
        return json_error("failed to open events file");
    }

    if (!capture_begin(pipefd, &saved_stdout)) {
        fclose(file);
        return json_error("failed to capture stdout");
    }
    longestSubyear(file, (char *)date1, (char *)date2);
    capture_end(pipefd, saved_stdout);
    fclose(file);
    return json_ok_output(g_capture);
}

const char *api_getInfoPersonality(const char *name) {
    TStack *result;
    const char *json;

    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    result = getInfoPersonality(g_stack, (char *)name);
    if (result == NULL) {
        return json_error("name not found in stack");
    }
    json = json_ok_stack_node(result);
    free(result);
    return json;
}

const char *api_sortNameStack(void) {
    ensure_loaded();
    g_stack = sortNameStack(g_stack);
    return json_ok_stack(g_stack);
}

const char *api_deleteName(const char *name) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_stack = deleteName(g_stack, (char *)name);
    return json_ok_stack(g_stack);
}

const char *api_updateStack(const char *name, const char *definition, const char *dob,
                            const char *dod) {
    ensure_loaded();
    if (name == NULL || name[0] == '\0') {
        return json_error("name is required");
    }

    g_stack = updateStack(g_stack,
                          (char *)name,
                          (char *)(definition != NULL ? definition : ""),
                          (char *)(dob != NULL ? dob : ""),
                          (char *)(dod != NULL ? dod : ""));
    return json_ok_stack(g_stack);
}

const char *api_sortEventStack(void) {
    TStack *events;

    ensure_loaded();
    events = active_event_stack();
    sortEventStack(events);
    return json_ok_stack(events);
}

const char *api_pronunciationStack(void) {
    TStack *short_stack = NULL;
    TStack *long_stack = NULL;
    const char *json;

    ensure_loaded();
    pronunciationStack(active_event_stack(), &short_stack, &long_stack);
    json = json_ok_pronunciation(short_stack, long_stack);
    free_stack_nodes(short_stack);
    free_stack_nodes(long_stack);
    return json;
}

const char *api_getSmallest(void) {
    char *smallest;
    const char *json;

    ensure_loaded();
    smallest = getSmallest(active_event_stack());
    if (smallest == NULL) {
        return json_error("getSmallest returned NULL");
    }

    json_reset();
    json_append_raw("{\"status\":\"ok\",\"data\":{\"value\":\"");
    json_append_escaped(smallest);
    json_append_raw("\"}}");
    json = g_json;
    free(smallest);
    return json;
}

const char *api_continuousSearch(void) {
    int pipefd[2] = {-1, -1};
    int saved_stdout;

    ensure_loaded();
    if (!capture_begin(pipefd, &saved_stdout)) {
        return json_error("failed to capture stdout");
    }
    continuousSearch(active_event_stack());
    capture_end(pipefd, saved_stdout);
    return json_ok_output(g_capture);
}

const char *api_isPersonalityKilled(const char *word) {
    if (word == NULL || word[0] == '\0') {
        return json_error("word is required");
    }
    return json_ok_bool("result", isPersonalityKilled((char *)word));
}

const char *api_recRevStack(void) {
    TStack *reversed;

    ensure_loaded();
    reversed = recRevStack(g_stack);
    free_stack_nodes(g_stack);
    g_stack = reversed;
    return json_ok_stack(g_stack);
}
