import type { editor, languages } from "monaco-editor";
import OneDarkPro from "../one-dark-pro.json";
import { Editor, type EditorProps, type Monaco } from "@monaco-editor/react";

import { VirtualSignalCompletor } from "@/completion/signal-completion";
import { Syntax } from "@/syntax";
import { TypeCompletor } from "@/completion/type-completions";

interface CodeEditorProps {
  native?: EditorProps;
}

export function CodeEditor({ native }: CodeEditorProps) {
  const handleEditorDidMount = (
    editorInstance: editor.IStandaloneCodeEditor,
    monaco: Monaco
  ) => {
    monaco.languages.register({ id: "compufactorio" });

    monaco.languages.setMonarchTokensProvider(
      "compufactorio",
      Syntax as languages.IMonarchLanguage
    );

    monaco.editor.defineTheme(
      "OneDarkPro",
      OneDarkPro as editor.IStandaloneThemeData
    );
    monaco.editor.setTheme("OneDarkPro");

    monaco.languages.registerCompletionItemProvider(
      "compufactorio",
      VirtualSignalCompletor(monaco)
    );

    monaco.languages.registerCompletionItemProvider(
      "compufactorio",
      TypeCompletor(monaco)
    );

    const model = editorInstance.getModel();
    if (model) {
      monaco.editor.setModelLanguage(model, "compufactorio");
    }
  };

  return (
    <Editor
      defaultLanguage="compufactorio"
      onMount={handleEditorDidMount}
      options={{
        acceptSuggestionOnCommitCharacter: false,
        suggestOnTriggerCharacters: true,
        quickSuggestions: true,
      }}
      {...native}
    />
  );
}
