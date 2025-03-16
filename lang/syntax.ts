import { VirtualSignals } from "./signals";

export const Syntax = {
  defaultToken: "invalid",

  keywords: ["vsignal", "out"],
  functions: ["sqrt"],
  types: [],
  enums: ["VirtualSignals"],
  enumMembers: [...Object.keys(VirtualSignals)],

  tokenizer: {
    root: [
      [
        /\b[a-zA-Z_]\w*(?=\s*\()/,
        {
          cases: {
            "@functions": "support.function",
            "@default": "identifier",
          },
        },
      ],
      [
        /\b[a-zA-Z_]\w*(?=::)/,
        {
          cases: {
            "@enums": "variable.language",
            "@default": "identifier",
          },
        },
      ],
      [
        /::/, "operator" 
      ],
      [
        /(::)([a-zA-Z_]\w*)/,
        ["operator", { cases: { "@enumMembers": "constant", "@default": "identifier" } }]
      ],
      [
        /\{/, "delimiter.bracket" 
      ],
      [
        /\}/, "delimiter.bracket" 
      ],
      [/\^|\+|-|\*|\/|=/, "operator"],
      [
        /\b[a-zA-Z_]\w*\b/,
        {
          cases: {
            "@keywords": "keyword",
            "@types": "meta.selector",
            "@enums": "variable.language",
            "@default": "identifier",
          },
        },
      ],
    ],
  },
};