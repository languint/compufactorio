import type { Monaco } from "@monaco-editor/react";
import type { languages } from "monaco-editor";

export function TypeCompletor(monaco: Monaco) {
  return {
    provideCompletionItems: (model, position) => {
      const suggestions: languages.CompletionItem[] = [];

      const textBeforeCursor = model.getValueInRange({
        startLineNumber: position.lineNumber,
        startColumn: Math.max(1, position.column - 8),
        endLineNumber: position.lineNumber,
        endColumn: position.column,
      });

      if (!textBeforeCursor.includes("v")) {
        return;
      }

      const wordInfo = model.getWordUntilPosition(position);
      const wordRange = new monaco.Range(
        position.lineNumber,
        wordInfo.startColumn,
        position.lineNumber,
        wordInfo.endColumn
      );

      const prefix = wordInfo.word;
      const additionalText = "vsignal".substring(prefix.length);

      suggestions.push({
        label: "vsignal",
        kind: monaco.languages.CompletionItemKind.Class,
        insertText: `${prefix}${additionalText}`,
        range: wordRange,
      });

      return { suggestions };
    },
  } as languages.CompletionItemProvider;
}
