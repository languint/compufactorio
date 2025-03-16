import type { Monaco } from "@monaco-editor/react";
import { VirtualSignals } from "lang/signals";
import type { languages, Range } from "monaco-editor";

export function VirtualSignalCompletor(monaco: Monaco) {
  return {
    triggerCharacters: [":"],
    provideCompletionItems: (model, position) => {
      const suggestions: languages.CompletionItem[] = [];

      const textBeforeCursor = model.getValueInRange({
        startLineNumber: position.lineNumber,
        startColumn: Math.max(1, position.column - 16),
        endLineNumber: position.lineNumber,
        endColumn: position.column,
      });

      if (textBeforeCursor.includes("V") && !textBeforeCursor.includes(":")) {
        const wordInfo = model.getWordUntilPosition(position);
        const wordRange = new monaco.Range(
          position.lineNumber,
          wordInfo.startColumn,
          position.lineNumber,
          wordInfo.endColumn
        );

        const prefix = wordInfo.word;
        const additionalText = "VirtualSignals".substring(prefix.length);

        suggestions.push({
          label: "VirtualSignals",
          kind: monaco.languages.CompletionItemKind.Enum,
          insertText: `${prefix}${additionalText}`,
          range: wordRange,
        });

        return { suggestions };
      }

      if (!textBeforeCursor.includes("VirtualSignals::")) {
        return;
      }

      Object.keys(VirtualSignals).forEach((key) =>
        suggestions.push({
          label: key,
          kind: monaco.languages.CompletionItemKind.EnumMember,
          insertText: key,
          detail: `Virtual Signal: ${key}`,
          range: {
            startLineNumber: position.lineNumber,
            startColumn: position.column,
            endLineNumber: position.lineNumber,
            endColumn: position.column,
          },
        })
      );

      return { suggestions };
    },
  } as languages.CompletionItemProvider;
}
