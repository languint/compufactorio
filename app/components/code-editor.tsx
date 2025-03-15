import type { editor } from "monaco-editor";
import OneDarkPro from "../one-dark-pro.json";
import { Editor, type EditorProps, type Monaco } from "@monaco-editor/react";

import { lib } from "lang/lib";
import { VirtualSignals } from "lang/signals";
import { VirtualSignalCompletor } from "lang/completion/signal-completion";

interface CodeEditorProps {
  native?: EditorProps;
}

export function CodeEditor({ native }: CodeEditorProps) {
  const handleEditorDidMount = (
    editor: editor.IStandaloneCodeEditor,
    monaco: Monaco
  ) => {
    monaco.editor.defineTheme(
      "OneDarkPro",
      OneDarkPro as editor.IStandaloneThemeData
    );

    monaco.editor.setTheme("OneDarkPro");

    monaco.languages.typescript.typescriptDefaults.addExtraLib(lib);

    monaco.languages.registerCompletionItemProvider(
      "typescript",
      VirtualSignalCompletor(monaco)
    );

    editor.onDidChangeModelContent(() => {
      editor.trigger("keyboard", "editor.action.triggerSuggest", {});
    });
  };

  return (
    <Editor
      defaultLanguage="typescript"
      onMount={handleEditorDidMount}
      {...native}
    />
  );
}
