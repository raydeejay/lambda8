(begin
  (define update
    (let ((t 0)
          (pi8 (/ pi 8))
          (pi2 (* pi 8)))
      (lambda ()
        (cls 0)

        ;; lines
        (do ((i (remainder t 8) (+ i 8)))
            ((> i 199))
          (line i 0 0 (- 199 i) 8)
          (line i 199 199 (- 199 i) 6)
          (set! t (+ t 0.01)))

        ;; prism
        (do ((i (remainder t 16) (+ i pi8)))
            ((> i pi2))
          (let ((x (+ 100 (* 32 (cos i))))
                (y (+ 100 (* 32 (cos i)))))
            (line 199 0 x y 15)
            (line 0 199 x y 15)))

        ;; ;; border
        (line 0 0 199 0 8)
        (line 0 0 0 199 8)
        (line 199 0 199 199 6)
        (line 0 199 199 199 6))))

  (display "lines demo loaded\n"))
