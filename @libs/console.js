// console.js

const Lib = LibraryManager.library;

// Partial source code reference from https://github.com/mame/xterm-pty/blob/main/emscripten-pty.js, MIT License

// Wrap fd_read to make stdin blocking
Lib['$real_fd_read'] = Lib['fd_read'];
Lib['$real_fd_read__deps'] = Lib['fd_read__deps'];
Object.assign(Lib, {
    fd_read__deps: ['$real_fd_read'],
    fd_read: (fd, iov, iovcnt, pnum) => Asyncify.handleSleep((wakeUp) => {
        let result = real_fd_read(fd, iov, iovcnt, pnum);
        // console.log('result', result);
        if (fd == 0 /* STDIN_FILENO */ && result == 6 /* EAGAIN */) {
            // console.log('wait');
            // Note: Module['console'] was called Module['tty'] before.
            Module['console'].waitForReadable().then(() => wakeUp(real_fd_read(fd, iov, iovcnt, pnum)));
        } else {
            setTimeout(() => wakeUp(result));
        }
    }),
    fd_read__async: true
});

// Override read & write to support xterm.js
Object.assign(Lib.$TTY.stream_ops, {
    read(stream, buffer, offset, length) {
        // console.log('read', stream);
        switch (stream.fd) {
            case 0:
                let readBytes = Module['console'].read(length);
                // console.log('read', readBytes);
                if (length != 0 && readBytes.length == 0) {
                    throw new FS.ErrnoError(6 /* EAGAIN */);
                }
                buffer.set(readBytes, offset);
                return readBytes.length;
            default:
                console.warn(`$TTY.stream_ops.write(): unexpected stream: stream.fd == ${stream.fd} (expected 0)`);
                return 0;
        }
    },
    write(stream, buffer, offset, length) {
        // console.log('write', stream);
        if (buffer === HEAP8) {
            buffer = HEAPU8;
        } else if (!(buffer instanceof Uint8Array)) {
            throw new Error(`$TTY.stream_ops.write(): unexpected buffer type: ${buffer.constructor.name}`);
        }
        switch (stream.fd) {
            case 1:
                Module['console'].write(Array.from(buffer.subarray(offset, offset + length)));
                return length;
            case 2:
                Module['console'].writeLog(new TextDecoder('utf-8').decode(buffer.subarray(offset, offset + length)));
                return length;
            default:
                console.warn(`$TTY.stream_ops.write(): unexpected stream: stream.fd == ${stream.fd} (expected 1 or 2)`);
                return 0;
        }
    }
});

// Note: replaced by a faster way.
/** @deprecated */
Object.assign(Lib, {
    $FS_stdin_getChar__deps: [],
    /** @deprecated */
    $FS_stdin_getChar() {
        console.warn('$FS_stdin_getChar() should never be called.');
        return Module['console'].getChar();
    }
});
Object.assign(Lib.$TTY.default_tty_ops, {
    /** @deprecated */
    get_char(tty) {
        console.warn('get_char() should never be called.');
        // let charCode = Module['tty'].getChar();
        // console.log('getchar', charCode);
        // return charCode;
        return Module['console'].getChar();
    },
    /** @deprecated */
    put_char(tty, val) {
        console.warn('put_char() should never be called.');
        // console.log('putchar', val);
        Module['console'].putChar(val);
    }
});

// Console APIs
addToLibrary({
    $doAsyncFunc: (impl) => Asyncify.handleSleep((wakeUp) => {
        // console.log('there');
        impl().then((res) => wakeUp(res));
    }),
    // _Impl_GetCharI
    _Impl_GetCharI__deps: ['$doAsyncFunc'],
    _Impl_GetCharI: () => doAsyncFunc(() => Module['console'].getCharWithoutEnter()),
    _Impl_GetCharI__async: true,
    // _Impl_SetRawMode
    _Impl_SetRawMode: (use) => {
        Module['console'].raw(use != 0 ? true : false);
    },
    // _Impl_GetConsoleSize
    _Impl_GetConsoleSize: () => Asyncify.handleSleep((wakeUp) => {
        setTimeout(() => {
            const { x, y } = Module['console'].getSize();
            // console.log(x, y, (x << 16) | (y));
            wakeUp((x << 16) | (y));
        });
    }),
    _Impl_GetConsoleSize__async: true,
});