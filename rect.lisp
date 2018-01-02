(begin
  ;; rect demo
  (define x 160)
  (define y 100)
  (define dx 7)
  (define dy 4)
  (define col 1)

  (cls 0)

  (define (update)
    ;;Update x/y
    (set! x (+ x dx))
    (set! y (+ y dy))

    ;;Check screen walls
    (when (or (< x 0) (> x (- SCREEN-WIDTH 6)))
          (set! dx (- dx))
          (set! col (+ 1 (remainder col 15))))

    (when (or (< y 0) (> y (- SCREEN-HEIGHT 6)))
          (set! dy (- dy))
          (set! col (+ 1 (remainder col 15))))
    
    ;;Draw rectangle
    (rect x y 6 6 col)))
