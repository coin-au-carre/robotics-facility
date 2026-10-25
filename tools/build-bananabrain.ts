import path from 'node:path'
import { pathToFileURL } from 'node:url'
import { buildNativeBot, nativeRecipePaths, validateOutputName } from './native-build.ts'
import type { NativeBuildOptions, NativeBuildRecipe } from './native-build.ts'

export const bananabrainRecipePaths = Object.freeze([
  ...nativeRecipePaths,
  'native/bananabrain.cmake',
  'tools/build-bananabrain.ts',
  'imports/bananabrain-aiide2025.json',
  'docs/bananabrain-prototype.md',
])

export const bananabrainRecipe: NativeBuildRecipe = {
  botId: 'bananabrain',
  target: 'BananaBrain',
  sourceIds: ['bwapi', 'bananabrain'],
  sourceVariables: { BWAPI_SOURCE_DIR: 'bwapi', BANANABRAIN_SOURCE_DIR: 'bananabrain' },
  outputVariable: 'BANANABRAIN_OUTPUT_DIR',
  recipePaths: bananabrainRecipePaths,
}

export function buildBananaBrain(options: NativeBuildOptions) {
  return buildNativeBot(bananabrainRecipe, options)
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const [outputName, ...rest] = process.argv.slice(2)
  if (!outputName || rest.length)
    throw new Error('Usage: node tools/build-bananabrain.ts <new-output-name>')
  buildBananaBrain({ outputName: validateOutputName(outputName) })
    .then(({ output }) => console.log(output))
    .catch(error => {
      console.error(error)
      process.exitCode = 1
    })
}
