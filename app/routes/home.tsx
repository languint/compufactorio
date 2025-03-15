import type { Route } from "./+types/home";
import { CodeEditor } from "~/components/code-editor";

export function meta({}: Route.MetaArgs) {
  return [{ title: "Compufactorio" }];
}

export default function Home() {
  return <div className="w-full h-full bg-background">
    <CodeEditor />
  </div>;
}