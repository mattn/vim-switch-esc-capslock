let s:lib = expand('<sfile>:h') . '/../hook.dll'

let s:handle = libcall('kernel32.dll', 'LoadLibraryA', s:lib)

function! SwitchEscCapslock(enable)
  call libcallnr(s:lib, 'SetKeyboardHook', a:enable)
  augroup SwitchEscCapslock
    au!
	autocmd VimLeave * call libcallnr(s:lib, 'SetKeyboardHook', 0)
  augroup END
endfunction
