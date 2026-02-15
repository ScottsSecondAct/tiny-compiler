" Vim syntax file for Tiny language
" Place in ~/.vim/syntax/tiny.vim
" Add to ~/.vim/ftdetect/tiny.vim:
"   autocmd BufRead,BufNewFile *.tiny set filetype=tiny

if exists("b:current_syntax")
    finish
endif

syn keyword tinyKeyword     fn let var return if else while for in print
syn keyword tinyType        int float bool string
syn keyword tinyBool        true false

syn match   tinyNumber      '\<\d\+\>'
syn match   tinyFloat       '\<\d\+\.\d\+\>'
syn match   tinyOperator    '[+\-*/%=<>!&|]'
syn region  tinyString      start='"' end='"' skip='\\"'
syn match   tinyComment     '//.*$'
syn region  tinyComment     start='/\*' end='\*/'

hi def link tinyKeyword     Statement
hi def link tinyType        Type
hi def link tinyBool        Boolean
hi def link tinyNumber      Number
hi def link tinyFloat       Float
hi def link tinyOperator    Operator
hi def link tinyString      String
hi def link tinyComment     Comment

let b:current_syntax = "tiny"
