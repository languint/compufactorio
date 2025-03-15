import type { editor } from "monaco-editor";
import OneDarkPro from "../one-dark-pro.json";
import { Editor, type EditorProps, type Monaco } from "@monaco-editor/react";

interface CodeEditorProps {
    native?: EditorProps
}

export function CodeEditor({native} : CodeEditorProps) {
  const handleEditorDidMount = (
    editor: editor.IStandaloneCodeEditor,
    monaco: Monaco
  ) => {
    monaco.editor.defineTheme(
      "OneDarkPro",
      OneDarkPro as editor.IStandaloneThemeData
    );
    monaco.editor.setTheme("OneDarkPro");
  };
  return (
    <Editor
      defaultLanguage="typescript"
      onMount={handleEditorDidMount}
      {...native}
    />
  );
}
