import process from 'child_process';
import { existsSync } from 'fs';
import { mkdir, opendir, readFile, rm, writeFile } from 'fs/promises';
import { promisify } from 'util';
import md5 from 'md5';
const exec = promisify(process.exec);

class App {
    id?: string;
    type: 'console';
    private?: boolean;
    name?: string;
    author?: string;
    license?: string;
    files?: {
        cpp?: Array<string>;
        js?: Array<string>;
    };
    compilerOptions?: {
        fallbackFlags?: Array<string>;
    };
    assets?: Array<string>;
}
const apps = new Array<App>;

const rootDir = await opendir('.');
for await (const appDir of rootDir) {
    if (appDir.isDirectory()) {
        const appJsonPath = `${rootDir.path}/${appDir.name}/app.json`;
        if (existsSync(appJsonPath)) {
            try {
                const app = JSON.parse(await readFile(appJsonPath, { encoding: 'utf-8' })) as App;
                app.id = appDir.name;
                apps.push(app);
            } catch (err) {
                console.log(`\x1B[1m\x1B[36mfile:\x1B[0m ${appJsonPath}`);
                console.log(`\x1B[1m\x1B[31merror:\x1B[0m ${err.toString().trim()}`);
                throw err;
            }
        }
    }
}

if (!existsSync('@build')) {
    await mkdir('@build');
}
async function build(app: App, sourceDir: string, buildDir: string, libraryDir: string) {
    try {
        if (existsSync(buildDir)) {
            await rm(buildDir, { recursive: true });
        }
        await mkdir(buildDir);
        let command =
            `emcc ${libraryDir}/impl.cpp -o ${buildDir}/main.mjs ` +
            `-I${libraryDir}/include --js-library=${libraryDir}/base.js -lidbfs.js ` +
            `-sWASM=1 -sMODULARIZE=1 -sEXPORT_ES6=1 -sASYNCIFY=1 -sFORCE_FILESYSTEM=1 -sALLOW_MEMORY_GROWTH=1 -sEXIT_RUNTIME=1 `;
        switch (app.type) {
            case 'console':
                command += `-DOLCPP_APP_TYPE_CONSOLE --js-library=${libraryDir}/console.js `;
                break;
        }
        const appHash = new Map<string, string>();
        for (const fileName of app.files.js) {
            command += `--js-library=${sourceDir}/${fileName} `;
            appHash.set(fileName, md5(await readFile(`${sourceDir}/${fileName}`)));
        }
        for (const fileName of app.files.cpp) {
            command += `${sourceDir}/${fileName} `;
            appHash.set(fileName, md5(await readFile(`${sourceDir}/${fileName}`)));
        }
        for (const fallbackFlag of app.compilerOptions.fallbackFlags) {
            command += `${fallbackFlag} `;
        }
        for (const assetFile of app.assets) {
            const sourceEncoded = `${sourceDir}/${assetFile}`.replaceAll('@', '@@');
            const assetEncoded = assetFile.replaceAll('@', '@@');
            command += `--preload-file=${sourceEncoded}@${assetEncoded} `;
            appHash.set(assetFile, md5(await readFile(`${sourceDir}/${assetFile}`)));
        }
        console.log(`\x1B[1m\x1B[37m- command:\x1B[0m ${command}`);
        await exec(command, { encoding: 'utf-8' });
        await writeFile(`${buildDir}/app.json`, JSON.stringify(app));
        await writeFile(`${buildDir}/app.files.json`, JSON.stringify([...appHash]));
        console.log(`\x1B[1m\x1B[32m+ build ${app.id} successfully!\x1B[0m`);
    } catch (err) {
        console.log(`\x1B[1m\x1B[31m! error:\x1B[0m ${err.toString().trim()}`);
        console.log(`\x1B[1m\x1B[31m! failed to build ${app.id}!`);
        console.log(`\x1B[1m\x1B[33m! warning:\x1B[0m\x1B[33m ${app.id} will not be included in the product.`);
        await rm(buildDir, { recursive: true });
        throw err;
    }
}
async function checkFiles(app: App, sourceDir: string, buildDir: string, libraryDir: string) {
    if (!existsSync(`${buildDir}/app.json`) || !existsSync(`${buildDir}/app.files.json`)) {
        // console.log(1);
        return false;
    }
    if ((await readFile(`${buildDir}/app.json`, { encoding: 'utf-8' })) != JSON.stringify(app)) {
        // console.log(2, JSON.parse(await readFile(`${buildDir}/app.json`, { encoding: 'utf-8' })), app);
        return false;
    }
    const hashMap = new Map<string, string>(JSON.parse(await readFile(`${buildDir}/app.files.json`, { encoding: 'utf-8' })));
    for (const [file, hash] of hashMap) {
        if (!(existsSync(`${sourceDir}/${file}`) && md5(await readFile(`${sourceDir}/${file}`)) == hash)) {
            // console.log(3, md5(await readFile(`${sourceDir}/${file}`)), hash);
            return false;
        }
    }
    return true;
}
let totalCount = 0, errorCount = 0;
const successApps = new Array<App>();
for (const app of apps) {
    try {
        const sourceDir = `${rootDir.path}/${app.id}`;
        const libraryDir = `${rootDir.path}/@libs`;
        const buildDir = `${rootDir.path}/@build/${app.id}`;
        console.log(`\x1B[1m\x1B[36m# building ${app.id}`);
        if (['console'].indexOf(app.type) != -1) {
            app.name ??= app.id;
            app.private ??= false;
            app.author ??= 'unknown';
            app.license ??= 'unknown';
            app.files ??= { cpp: ['main.cpp'], js: [] };
            app.files.cpp ??= ['main.cpp'];
            app.files.js ??= [];
            app.compilerOptions ??= { fallbackFlags: [] };
            app.compilerOptions.fallbackFlags ??= [];
            app.assets ??= [];
            console.log(`\x1B[1m\x1B[37m- type:\x1B[0m ${app.type}`);
            console.log(`\x1B[1m\x1B[37m- author:\x1B[0m ${app.author}`);
            console.log(`\x1B[1m\x1B[37m- sourceDir:\x1B[0m ${sourceDir}`);
            console.log(`\x1B[1m\x1B[37m- buildDir:\x1B[0m ${buildDir}`);
            if (existsSync(buildDir) && await checkFiles(app, sourceDir, buildDir, libraryDir)) {
                console.log(`\x1B[1m\x1B[33m+ note:\x1B[0m\x1B[33m ${app.id} has not changed, so it will not be built.`);
            } else {
                await build(app, sourceDir, buildDir, libraryDir);
            }
        }
        if (!app.private) {
            successApps.push(app);
        }
    } catch (err) {
        errorCount++;
    }
    totalCount++;
}
await writeFile(`@build/apps.json`, JSON.stringify(successApps));
console.log(`\x1B[1m\x1B[37m* error:\x1B[0m ${errorCount}\n  \x1B[1m\x1B[37mtotal:\x1B[0m ${totalCount}`);