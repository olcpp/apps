// base.js

addToLibrary({
    $doAsyncFunc: (impl) => Asyncify.handleSleep((wakeUp) => {
        // console.log('there');
        impl().then((res) => wakeUp(res));
    })
});

// _Impl_GetKeyState
addToLibrary({
    _Impl_GetKeyState__deps: ['$doAsyncFunc'],
    _Impl_GetKeyState: (keyCode) => doAsyncFunc(() => Module['base'].getKeyState(keyCode)),
    _Impl_GetKeyState__async: true,
    _Impl_InitIDBFS: (mountpoint) => Asyncify.handleSleep((wakeUp) => {
        const mpstr = UTF8ToString(mountpoint);
        FS.mkdir(mpstr);
        FS.mount(IDBFS, { autoPersist: true }, mpstr);
        FS.syncfs(true, (err) => {
            if (err) {
                throw err;
            }
            wakeUp();
        });
    }),
    _Impl_InitIDBFS__async: true,
    _Impl_OpenURL: (url) => Asyncify.handleSleep((wakeUp) => {
        setTimeout(() => {
            const urlstr = UTF8ToString(url);
            window.open(urlstr, "_blank");
            wakeUp();
        });
    }),
    _Impl_OpenURL__async: true,
});