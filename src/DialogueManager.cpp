#include "DialogueManager.h"

DialogueManager::DialogueManager()
    : active(false),
dialogueLines(),
currentLine(0){

}

void DialogueManager::StartDialogue(const std::string& text) {

    dialogueLines.clear();

    dialogueLines.push_back(text);

    currentLine = 0;
    active = true;
}

void DialogueManager::StartDialogue(
    const std::vector<std::string>& lines) {

   dialogueLines = lines;

    currentLine = 0;

    if (dialogueLines.empty()) {
        active = false;
        return;
    }
    active = true;
}

void DialogueManager::StartDialogue(
const DialogueData& dialogue) {

    dialogueLines = dialogue.lines;

    currentLine = 0;

    if (dialogueLines.empty()) {
        active = false;
        return;
    }

    active = true;
}

void DialogueManager::AdvanceDialogue() {

    if (!active || dialogueLines.empty()) {
        return;
    }

    if (currentLine + 1 < dialogueLines.size()) {
        currentLine++;
    }
    else {
        StopDialogue();
    }
}

void DialogueManager::StopDialogue() {

    active = false;
    dialogueLines.clear();
    currentLine = 0;
}

bool DialogueManager::IsActive() const {
    return active;
}

const std::string& DialogueManager::GetCurrentText() const {

    static const std::string emptyText;

    if (dialogueLines.empty()) {
        return emptyText;
    }

    return dialogueLines[currentLine];
}