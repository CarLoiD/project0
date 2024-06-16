" Default Colorscheme
colorscheme zx

" Misc buffer options
set number relativenumber
set splitright
set tabstop=4
set shiftwidth=4

" Disable swap file creation
set noswapfile

autocmd FileType make set noexpandtab

augroup source_files
	autocmd!
	autocmd BufNewFile,BufRead *.cpp 	set expandtab
	autocmd BufNewFile,BufRead *.h 		set expandtab
	autocmd BufNewFile,BufRead *.c 		set expandtab
	autocmd BufNewFile,BufRead *.S 		set expandtab
	autocmd BufNewFile,BufRead *.vu0	set expandtab
	autocmd BufNewFile,BufRead *.vu1 	set expandtab
augroup END

augroup number_toggle
	autocmd!
	autocmd BufEnter,FocusGained,InsertLeave,WinEnter * if &nu && mode() != "i" | set rnu   | endif
	autocmd BufLeave,FocusLost,InsertEnter,WinLeave   * if &nu                  | set nornu | endif
augroup END

nmap <C-n> :vertical res +2<CR>
nmap <C-m> :vertical res -2<CR>
