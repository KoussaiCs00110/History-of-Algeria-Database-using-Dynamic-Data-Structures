const GROUPS = [
  {
    label: "BST",
    note: "",
    items: [
      {
        name: "insertToBST",
        hint: "Insert or update a node.",
        payload: {
          name: "Test Node",
          definition: "Temporary definition from the web bridge",
          dob: "01/01/1900",
          dod: "01/01/1950",
          event_date: ""
        }
      },
      {
        name: "fillTree",
        hint: "Build tree from CSV.",
        payload: {
          csv_text:
            "Sample One,Definition one,01/01/1900,01/01/1950,\n" +
            "Sample Two,Definition two,02/02/1902,03/03/1960,"
        },
        fieldTypes: { csv_text: "textarea" },
        fieldRows: { csv_text: 8 }
      },
      {
        name: "getInfoNameTree",
        hint: "Search by name.",
        payload: { name: "Emir Abdelkader" }
      },
      {
        name: "addNameBST",
        hint: "Add a name to the tree.",
        payload: { name: "Bridge BST Entry", dob: "01/01/1901", dod: "01/01/1971" }
      },
      {
        name: "deleteNameBST",
        hint: "Delete by name.",
        payload: { name: "Bridge BST Entry" }
      },
      {
        name: "updateNameBST",
        hint: "Update one tree node.",
        payload: {
          name: "Emir Abdelkader",
          definition: "Updated through the wrapper",
          dob: "23/09/1808",
          dod: "26/05/1883"
        }
      },
      {
        name: "traversalBSTinOrder",
        hint: "Show in-order traversal.",
        payload: {}
      }
    ]
  },
  {
    label: "Linked List",
    note: "",
    items: [
      { name: "getPersonality", hint: "Show personality list.", payload: {} },
      { name: "getDatePersonality", hint: "Show date list.", payload: {} },
      { name: "getInfoByDates", hint: "Search by birth date.", payload: { dob: "23/09/1808" } },
      { name: "getInfoByDates2", hint: "Search by death date.", payload: { dod: "26/05/1883" } },
      { name: "sortWord", hint: "Sort by name.", payload: {} },
      { name: "sortWord2", hint: "Sort by name length.", payload: {} },
      { name: "convert", hint: "Convert date text.", payload: { date_text: "23/09/1808" } },
      { name: "agecalculate", hint: "Calculate age.", payload: { start_date: "23/09/1808", end_date: "26/05/1883" } },
      { name: "sortPersonality", hint: "Sort by age.", payload: {} },
      { name: "deletepersonality", hint: "Delete one name.", payload: { name: "Bridge Person" } },
      {
        name: "updatePersonality",
        hint: "Update one name.",
        payload: {
          name: "Emir Abdelkader",
          definition: "Updated from the linked-list wrapper",
          dob: "23/09/1808",
          dod: "26/05/1883"
        }
      },
      { name: "similarPersonality", hint: "Find similar dates.", payload: { word: "1957" } },
      { name: "countPersonality", hint: "Count by date.", payload: { date_text: "23/09/1808" } },
      { name: "isPalindrome", hint: "Check palindrome.", payload: { word: "level" } },
      { name: "palindromeName", hint: "Show palindrome names.", payload: {} },
      { name: "mergeNodes", hint: "Merge the two lists.", payload: {} },
      { name: "merge2Nodes", hint: "Merge circular list.", payload: {} },
      {
        name: "addPersonality",
        hint: "Add one personality.",
        payload: {
          name: "Bridge Person",
          definition: "Temporary record created through the web bridge",
          dob: "01/01/1905",
          dod: "01/01/1980"
        }
      },
      {
        name: "addEvents",
        hint: "Add one event.",
        payload: {
          name: "Bridge Event",
          date: "17/05/2026"
        }
      }
    ]
  },
  {
    label: "Recursion",
    note: "",
    items: [
      { name: "countOccurence", hint: "Count matches in file.", payload: { name: "Emir Abdelkader" } },
      { name: "removeOccurrence", hint: "Preview remove result.", payload: { word: "Emir Abdelkader" } },
      {
        name: "replaceOccurence",
        hint: "Preview replace result.",
        payload: {
          name: "Emir Abdelkader",
          definition: "Replacement definition",
          dob: "23/09/1808",
          dod: "26/05/1883"
        }
      },
      { name: "namePermutation", hint: "Show permutations.", payload: { name: "Ali" } },
      { name: "printSub", hint: "Run printSub helper.", payload: { word: "war", res: "", i: 0, j: 0 } },
      { name: "subseqName", hint: "Show subsequences.", payload: { word: "war" } },
      { name: "distinctSubseqWord", hint: "Count subsequences.", payload: { event: "war" } },
      { name: "isPalindromeWord", hint: "Check palindrome range.", payload: { event: "level", i: 0, j: 4 } },
      { name: "longestSubyear", hint: "Search date range.", payload: { date1: "01/01/1954", date2: "31/12/1962" } }
    ]
  },
  {
    label: "Stack",
    note: "",
    items: [
      { name: "getInfoPersonality", hint: "Search stack by name.", payload: { name: "Emir Abdelkader" } },
      { name: "sortNameStack", hint: "Sort stack by name.", payload: {} },
      { name: "deleteName", hint: "Delete from stack.", payload: { name: "Bridge Person" } },
      {
        name: "updateStack",
        hint: "Update stack node.",
        payload: {
          name: "Emir Abdelkader",
          definition: "Updated from the stack wrapper",
          dob: "23/09/1808",
          dod: "26/05/1883"
        }
      },
      { name: "sortEventStack", hint: "Sort event stack.", payload: {} },
      { name: "pronunciationStack", hint: "Split short and long.", payload: {} },
      { name: "getSmallest", hint: "Get smallest value.", payload: {} },
      { name: "continuousSearch", hint: "Show overlaps.", payload: {} },
      { name: "isPersonalityKilled", hint: "Check for 'killed'.", payload: { word: "captured and killed by French forces" } },
      { name: "recRevStack", hint: "Reverse stack.", payload: {} }
    ]
  }
];

const DEFAULT_FUNCTION = "getInfoNameTree";

const FIELD_LABELS = {
  name: "Name",
  definition: "Definition",
  dob: "Date of Birth",
  dod: "Date of Death",
  date: "Event Date",
  date1: "Start Date",
  date2: "End Date",
  date_text: "Date",
  start_date: "Start Date",
  end_date: "End Date",
  event_date: "Event Date",
  csv_text: "CSV Content",
  word: "Word",
  event: "Event Text",
  res: "Initial Result Buffer",
  i: "Start Index",
  j: "End Index"
};

const FIELD_HELP = {
  dob: "DD/MM/YYYY",
  dod: "DD/MM/YYYY",
  date: "DD/MM/YYYY",
  date1: "DD/MM/YYYY",
  date2: "DD/MM/YYYY",
  date_text: "DD/MM/YYYY",
  start_date: "DD/MM/YYYY",
  end_date: "DD/MM/YYYY",
  event_date: "Optional",
  csv_text: "One row per line",
  res: "Usually leave this empty",
  i: "Starts at 0",
  j: "Starts at 0"
};

const FIELD_PLACEHOLDERS = {
  name: "Enter a name",
  definition: "Write a definition",
  dob: "23/09/1808",
  dod: "26/05/1883",
  date: "17/05/2026",
  date1: "01/01/1954",
  date2: "31/12/1962",
  date_text: "23/09/1808",
  start_date: "23/09/1808",
  end_date: "26/05/1883",
  event_date: "Optional event date",
  csv_text: "name,definition,DoB,DoD,event_date",
  word: "Enter a word",
  event: "Enter event text",
  res: "Optional starting buffer"
};

const operationMap = new Map();
const sidebarNav = document.getElementById("sidebarNav");
const groupLabel = document.getElementById("groupLabel");
const functionTitle = document.getElementById("functionTitle");
const hint = document.getElementById("hint");
const functionForm = document.getElementById("functionForm");
const resetButton = document.getElementById("resetButton");
const emptyState = document.getElementById("emptyState");
const resultOutput = document.getElementById("resultOutput");
const statusPill = document.getElementById("statusPill");

let selectedFunctionName = DEFAULT_FUNCTION;

GROUPS.forEach((group) => {
  group.items.forEach((item) => {
    item.groupLabel = group.label;
    item.groupNote = group.note;
    operationMap.set(item.name, item);
  });
});

if (!operationMap.has(selectedFunctionName)) {
  selectedFunctionName = GROUPS[0].items[0].name;
}

function setStatus(label, kind = "") {
  statusPill.textContent = label;
  statusPill.className = `pill ${kind}`.trim();
}

function prettyLabel(key) {
  if (FIELD_LABELS[key]) {
    return FIELD_LABELS[key];
  }

  return key
    .replace(/_/g, " ")
    .replace(/\b\w/g, (char) => char.toUpperCase());
}

function fieldHelpText(key) {
  return FIELD_HELP[key] || "";
}

function fieldPlaceholder(key) {
  return FIELD_PLACEHOLDERS[key] || `Enter ${prettyLabel(key).toLowerCase()}`;
}

function inferFieldType(item, key, value) {
  if (item.fieldTypes && item.fieldTypes[key]) {
    return item.fieldTypes[key];
  }
  if (typeof value === "number") {
    return "number";
  }
  if (key === "definition" || key === "csv_text") {
    return "textarea";
  }
  return "text";
}

function fieldRows(item, key) {
  if (item.fieldRows && item.fieldRows[key]) {
    return item.fieldRows[key];
  }
  if (key === "definition") {
    return 5;
  }
  return 4;
}

function activeItem() {
  return operationMap.get(selectedFunctionName);
}

function renderSidebar() {
  sidebarNav.innerHTML = "";

  GROUPS.forEach((group) => {
    const groupSection = document.createElement("section");
    groupSection.className = "nav-group";

    const title = document.createElement("h3");
    title.textContent = group.label;
    groupSection.appendChild(title);

    if (group.note) {
      const note = document.createElement("p");
      note.className = "nav-note";
      note.textContent = group.note;
      groupSection.appendChild(note);
    }

    const list = document.createElement("div");
    list.className = "nav-list";

    group.items.forEach((item) => {
      const button = document.createElement("button");
      button.type = "button";
      button.className = "nav-button";
      button.dataset.function = item.name;
      button.textContent = item.name;
      button.addEventListener("click", () => selectFunction(item.name));
      list.appendChild(button);
    });

    groupSection.appendChild(list);
    sidebarNav.appendChild(groupSection);
  });

  updateSidebarActiveState();
}

function updateSidebarActiveState() {
  document.querySelectorAll(".nav-button").forEach((button) => {
    button.classList.toggle("active", button.dataset.function === selectedFunctionName);
  });
}

function renderForm(item) {
  const payloadKeys = Object.keys(item.payload);

  functionForm.innerHTML = "";
  functionForm.style.display = payloadKeys.length === 0 ? "none" : "grid";
  emptyState.style.display = payloadKeys.length === 0 ? "block" : "none";

  payloadKeys.forEach((key) => {
    const wrapper = document.createElement("label");
    wrapper.className = "field";

    const title = document.createElement("span");
    title.className = "field-label";
    title.textContent = prettyLabel(key);
    wrapper.appendChild(title);

    const value = item.payload[key];
    const type = inferFieldType(item, key, value);
    let control;

    if (type === "textarea") {
      control = document.createElement("textarea");
      control.rows = fieldRows(item, key);
      control.value = value;
    } else {
      control = document.createElement("input");
      control.type = type;
      control.value = String(value);
      if (type === "number") {
        control.step = "1";
      }
    }

    control.name = key;
    control.id = `field-${key}`;
    control.placeholder = fieldPlaceholder(key);
    wrapper.appendChild(control);

    const help = fieldHelpText(key);
    if (help) {
      const note = document.createElement("small");
      note.textContent = help;
      wrapper.appendChild(note);
    }

    functionForm.appendChild(wrapper);
  });
}

function renderSelectedFunction() {
  const item = activeItem();
  if (!item) {
    return;
  }

  groupLabel.textContent = item.groupLabel;
  functionTitle.textContent = item.name;
  hint.textContent = item.hint;
  renderForm(item);
  updateSidebarActiveState();
}

function selectFunction(name) {
  selectedFunctionName = name;
  renderSelectedFunction();
  setStatus("idle");
  const activeButton = document.querySelector(`.nav-button[data-function="${name}"]`);
  if (activeButton) {
    activeButton.scrollIntoView({ block: "nearest" });
  }
}

function collectPayload(item) {
  const payload = {};

  Object.entries(item.payload).forEach(([key, defaultValue]) => {
    const field = functionForm.elements.namedItem(key);
    const rawValue = field ? field.value : defaultValue;

    if (typeof defaultValue === "number") {
      payload[key] = rawValue === "" ? 0 : Number(rawValue);
    } else {
      payload[key] = rawValue;
    }
  });

  return payload;
}

async function sendRequest(event) {
  event.preventDefault();

  const item = activeItem();
  if (!item) {
    return;
  }

  const payload = collectPayload(item);
  setStatus("loading");
  resultOutput.textContent = "Loading...";

  try {
    const response = await fetch(`/api/${encodeURIComponent(item.name)}`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(payload)
    });

    const text = await response.text();
    let data;

    try {
      data = JSON.parse(text);
    } catch (parseError) {
      throw new Error(text || "Server did not return JSON.");
    }

    setStatus(data.status || "done", data.status === "ok" ? "ok" : "error");
    resultOutput.textContent = JSON.stringify(data, null, 2);
  } catch (error) {
    setStatus("request failed", "error");
    resultOutput.textContent = JSON.stringify(
      { status: "error", message: error.message },
      null,
      2
    );
  }
}

function resetFields() {
  renderSelectedFunction();
}

renderSidebar();
renderSelectedFunction();
setStatus("idle");
resultOutput.textContent = JSON.stringify(
  { status: "ok", data: "Pick a function and run it." },
  null,
  2
);

functionForm.addEventListener("submit", sendRequest);
resetButton.addEventListener("click", resetFields);
