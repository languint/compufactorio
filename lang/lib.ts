export const lib = `
declare interface Signal {
  name: string;
  iconId: string;
}

declare const VirtualSignals: { [k: string]: Signal };
`