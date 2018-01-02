;; this is the Lisp side of the editor

(begin
  (define ideal-column #f)
  
  (define (char-forward)
    (point-move 1)
    (set! ideal-column #f))
  
  (define (char-backward)
    (point-move -1)
    (set! ideal-column #f))

  (define (word-forward)
    (find-first-not-in-forward ",._ \n")
    (find-first-in-forward ",._ \n"))
  
  (define (word-backward)
    (find-first-not-in-backward ",._ \n")
    (find-first-in-backward ",._ \n")
    (point-move 1))

  (define (line-up)
    (when (not ideal-column)
          (set! ideal-column (get-column)))
    (find-first-in-backward "\n")
    (let ((end (get-column)))
      (when (> end ideal-column)
            (point-move (- 0 (- end ideal-column))))))
  
  (define (line-down)
    (when (not ideal-column)
          (set! ideal-column (get-column)))
    (point-move -1)
    (find-first-in-forward "\n")
    (find-first-in-forward "\n")
    (when ideal-column
          (let ((end (get-column)))
            (when (< ideal-column end)
                  (point-move (- 0 (- end ideal-column)))))))
  
  (define keybindings
    (hash-table* "a" #t
                 "b" #t
                 "c" #t
                 "d" #t
                 "e" #t
                 "f" #t
                 "g" #t
                 "h" #t
                 "i" #t
                 "j" #t
                 "k" #t
                 "l" #t
                 "m" #t
                 "n" #t
                 "o" #t
                 "p" #t
                 "q" #t
                 "r" #t
                 "s" #t
                 "t" #t
                 "u" #t
                 "v" #t
                 "w" #t
                 "x" #t
                 "y" #t
                 "z" #t
                 "1" #t
                 "2" #t
                 "3" #t
                 "4" #t
                 "5" #t
                 "6" #t
                 "7" #t
                 "8" #t
                 "9" #t
                 "0" #t
                 "," #t
                 "." #t
                 "\\" #t
                 "/" #t
                 "(" #t
                 ")" #t
                 ";" #t
                 "-" #t
                 "=" #t
                 "'" #t
                 "A" #t
                 "B" #t
                 "C" #t
                 "D" #t
                 "E" #t
                 "F" #t
                 "G" #t
                 "H" #t
                 "I" #t
                 "J" #t
                 "K" #t
                 "L" #t
                 "M" #t
                 "N" #t
                 "O" #t
                 "P" #t
                 "Q" #t
                 "R" #t
                 "S" #t
                 "T" #t
                 "U" #t
                 "V" #t
                 "W" #t
                 "X" #t
                 "Y" #t
                 "Z" #t
                 " " #t
                 "!" #t
                 "@" #t
                 "#" #t
                 "$" #t
                 "%" #t
                 "^" #t
                 "&" #t
                 "*" #t
                 "{" #t
                 "}" #t
                 "<" #t
                 ">" #t
                 "?" #t
                 "|" #t
                 "[" #t
                 "]" #t
                 ":" #t
                 "_" #t
                 "+" #t
                 
                 "<up>"    line-up
                 "<down>"  line-down
                 "<left>"  char-backward
                 "<right>" char-forward

                 "C-f"     char-forward
                 "C-b"     char-backward
                 "M-f"     word-forward
                 "M-b"     word-backward
                 ))

  (define (shift-letter key) (string-upcase key))

  (define (shift-number key)
    ('("}" "!" "@" "#" "$" "%" "^" "&" "*" "{") (- (char->integer key) #x30)))

  (define (shift-other key)
    (case key
      ((#\,) "<")
      ((#\.) ">")
      ((#\/) "?")
      ((#\-) "_")
      ((#\=) "+")
      ((#\\) "|")
      ((#\() "[")
      ((#\)) "]")
      ((#\;) ":")
      (else key)))
  
  (define (encode-key key modifiers)
    ;;(display "KEY:" key "MODS:" modifiers)
    (when (= (logand modifiers 1) 1)
          (let ((c (key 0)))
            (if (<= (char->integer #\a) (char->integer c) (char->integer #\z))
                (set! key (shift-letter key))
                (if (<= (char->integer #\0) (char->integer c) (char->integer #\9))
                    (set! key (shift-number c))
                    (set! key (shift-other c))))))
    (when (= (logand modifiers 4) 4)
          (set! key (format "M-~a" key)))
    (when (= (logand modifiers 2) 2)
          (set! key (format "C-~a" key)))
    key)

  (define (handle-key key modifiers)
    (let ((k (encode-key key modifiers)))
      (let ((func (keybindings k)))
        (if func
            (if (equal? func #t)
                (insert-char k)
                (func))))))

  (display "Editor loaded!\n")
  #t)
