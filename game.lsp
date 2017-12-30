(do
 ;; game proper begins here
 (= left (define-sprite "left.png"))
 (= right (define-sprite "right.png"))
 
 (= fail-sfx (define-sfx "fail.wav"))

  ;; each ball is represented as a cons
  ;; the CAR is the index in its positions array,
  ;; the CDR is its current direction
  (= update
     (let (tick 0
                score 0
                x 0 y 0
                pose right)
       (fn (i)
           ;; react to input
           (when (btnp 3)
             (when (is pose right)
               (= pose left)))
           (when (btnp 2)
             (when (is pose left)
               (= pose right)))

           ;; draw player
           (spr pose)

           (printxy score (- SCREEN-WIDTH 40) 0)
           (++ tick))))

 nil)
