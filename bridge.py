import ctypes
import json
import os


LIB_PATH = os.environ.get("HISTORY_BRIDGE_SO", "/tmp/history_bridge.so")

CALLS = {
    "insertToBST": {"symbol": "api_insertToBST", "args": [("name", "str"), ("definition", "str"), ("dob", "str"), ("dod", "str"), ("event_date", "str")]},
    "fillTree": {"symbol": "api_fillTree", "args": [("csv_text", "str")]},
    "getInfoNameTree": {"symbol": "api_getInfoNameTree", "args": [("name", "str")]},
    "addNameBST": {"symbol": "api_addNameBST", "args": [("name", "str"), ("dob", "str"), ("dod", "str")]},
    "deleteNameBST": {"symbol": "api_deleteNameBST", "args": [("name", "str")]},
    "updateNameBST": {"symbol": "api_updateNameBST", "args": [("name", "str"), ("definition", "str"), ("dob", "str"), ("dod", "str")]},
    "traversalBSTinOrder": {"symbol": "api_traversalBSTinOrder", "args": []},
    "getPersonality": {"symbol": "api_getPersonality", "args": []},
    "getDatePersonality": {"symbol": "api_getDatePersonality", "args": []},
    "getInfoByDates": {"symbol": "api_getInfoByDates", "args": [("dob", "str")]},
    "getInfoByDates2": {"symbol": "api_getInfoByDates2", "args": [("dod", "str")]},
    "sortWord": {"symbol": "api_sortWord", "args": []},
    "sortWord2": {"symbol": "api_sortWord2", "args": []},
    "convert": {"symbol": "api_convert", "args": [("date_text", "str")]},
    "agecalculate": {"symbol": "api_agecalculate", "args": [("start_date", "str"), ("end_date", "str")]},
    "sortPersonality": {"symbol": "api_sortPersonality", "args": []},
    "deletepersonality": {"symbol": "api_deletepersonality", "args": [("name", "str")]},
    "updatePersonality": {"symbol": "api_updatePersonality", "args": [("name", "str"), ("definition", "str"), ("dob", "str"), ("dod", "str")]},
    "similarPersonality": {"symbol": "api_similarPersonality", "args": [("word", "str")]},
    "countPersonality": {"symbol": "api_countPersonality", "args": [("date_text", "str")]},
    "isPalindrome": {"symbol": "api_isPalindrome", "args": [("word", "str")]},
    "palindromeName": {"symbol": "api_palindromeName", "args": []},
    "mergeNodes": {"symbol": "api_mergeNodes", "args": []},
    "merge2Nodes": {"symbol": "api_merge2Nodes", "args": []},
    "addPersonality": {"symbol": "api_addPersonality", "args": [("name", "str"), ("definition", "str"), ("dob", "str"), ("dod", "str")]},
    "addEvents": {"symbol": "api_addEvents", "args": [("name", "str"), ("date", "str")]},
    "countOccurence": {"symbol": "api_countOccurence", "args": [("name", "str")]},
    "removeOccurrence": {"symbol": "api_removeOccurrence", "args": [("word", "str")]},
    "replaceOccurence": {"symbol": "api_replaceOccurence", "args": [("name", "str"), ("definition", "str"), ("dob", "str"), ("dod", "str")]},
    "namePermutation": {"symbol": "api_namePermutation", "args": [("name", "str")]},
    "printSub": {"symbol": "api_printSub", "args": [("word", "str"), ("res", "str"), ("i", "int"), ("j", "int")]},
    "subseqName": {"symbol": "api_subseqName", "args": [("word", "str")]},
    "distinctSubseqWord": {"symbol": "api_distinctSubseqWord", "args": [("event", "str")]},
    "isPalindromeWord": {"symbol": "api_isPalindromeWord", "args": [("event", "str"), ("i", "int"), ("j", "int")]},
    "longestSubyear": {"symbol": "api_longestSubyear", "args": [("date1", "str"), ("date2", "str")]},
    "getInfoPersonality": {"symbol": "api_getInfoPersonality", "args": [("name", "str")]},
    "sortNameStack": {"symbol": "api_sortNameStack", "args": []},
    "deleteName": {"symbol": "api_deleteName", "args": [("name", "str")]},
    "updateStack": {"symbol": "api_updateStack", "args": [("name", "str"), ("definition", "str"), ("dob", "str"), ("dod", "str")]},
    "sortEventStack": {"symbol": "api_sortEventStack", "args": []},
    "pronunciationStack": {"symbol": "api_pronunciationStack", "args": []},
    "getSmallest": {"symbol": "api_getSmallest", "args": []},
    "continuousSearch": {"symbol": "api_continuousSearch", "args": []},
    "isPersonalityKilled": {"symbol": "api_isPersonalityKilled", "args": [("word", "str")]},
    "recRevStack": {"symbol": "api_recRevStack", "args": []},
}

TYPE_MAP = {
    "str": ctypes.c_char_p,
    "int": ctypes.c_int,
}


def _load_library():
    try:
        lib = ctypes.CDLL(LIB_PATH)
    except OSError as exc:
        hint = (
            "Shared library not found. Build it first, for example:\n"
            "gcc -shared -fPIC -Iinclude main_api.c src/linked_list.c src/stack.c "
            "src/bst.c src/recursion.c -o /tmp/history_bridge.so -lm"
        )
        raise RuntimeError(f"{exc}\n{hint}") from exc

    for spec in CALLS.values():
        fn = getattr(lib, spec["symbol"])
        fn.restype = ctypes.c_char_p
        fn.argtypes = [TYPE_MAP[arg_type] for _, arg_type in spec["args"]]

    return lib


_LIB = _load_library()


def _marshal(payload, spec):
    args = []
    payload = payload or {}

    for key, arg_type in spec["args"]:
        if arg_type == "str":
            value = payload.get(key, "")
            if value is None:
                value = ""
            args.append(str(value).encode("utf-8"))
        else:
            value = payload.get(key, 0)
            args.append(int(value))

    return args


def call_bridge(function_name, payload=None):
    spec = CALLS.get(function_name)
    if spec is None:
        return {"status": "error", "message": f"Unsupported function: {function_name}"}

    fn = getattr(_LIB, spec["symbol"])
    raw = fn(*_marshal(payload, spec))
    if raw is None:
        return {"status": "error", "message": f"{function_name} returned no data"}

    text = raw.decode("utf-8")
    try:
        return json.loads(text)
    except json.JSONDecodeError:
        return {"status": "error", "message": "Wrapper returned invalid JSON", "raw": text}
