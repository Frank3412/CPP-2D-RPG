#include "DialogueManager.h"

DialogueManager::DialogueManager()
    : active(false),
currentText("") {

}

void DialogueManager::StartDialogue(const std::string& text) {
    currentText = text;
    active = true;
}

void DialogueManager::StopDialogue() {
    active = false;
    currentText.clear();
}

bool DialogueManager::IsActive() const {
    return active;
}

const std::string& DialogueManager::GetCurrentText() const {
    return currentText;
}