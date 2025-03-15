import type { Monaco } from "@monaco-editor/react";
import { VirtualSignals } from "lang/signals";
import type { languages } from "monaco-editor";

export function VirtualSignalCompletor(monaco: Monaco) {
  return {
    triggerCharacters: ["."],
    provideCompletionItems: (model, position) => {
      const textBeforeCursor = model.getValueInRange({
        startLineNumber: position.lineNumber,
        startColumn: Math.max(1, position.column - 15),
        endLineNumber: position.lineNumber,
        endColumn: position.column,
      });

      if (!textBeforeCursor.includes("VirtualSignals")) {
        return;
      }

      const suggestions = Object.keys(VirtualSignals).map((key) => ({
        label: key,
        kind: monaco.languages.CompletionItemKind.Constant,
        insertText: key,
        detail: `Virtual Signal: ${key}`,
        range: {
          startLineNumber: position.lineNumber,
          startColumn: position.column,
          endLineNumber: position.lineNumber,
          endColumn: position.column,
        },
      }));

      return { suggestions };
    },
  } as languages.CompletionItemProvider;
}
