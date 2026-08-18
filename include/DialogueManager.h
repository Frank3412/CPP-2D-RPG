#pragma once

#include <string>

class DialogueManager {
    public:
    DialogueManager();

    void StartDialogue(const std::string& text);
    void StopDialogue();

    bool IsActive() const;

    const std::string& GetCurrentText() const;

private:
    bool active;
    std::string currentText;
};
