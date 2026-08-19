#pragma once

#include <string>
#include <vector>

class DialogueManager {
    public:
    DialogueManager();

    void StartDialogue(const std::string& text);
    void StartDialogue(const std::vector<std::string>& lines);

    void AdvanceDialogue();
    void StopDialogue();

    bool IsActive() const;

    const std::string& GetCurrentText() const;

private:
    bool active;
    std::vector<std::string> dialogueLines;
    size_t currentLine;

};
