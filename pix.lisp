(begin
  (cls 0)
  (define (update)
    (do ((i 0 (+ i 1)))
        ((= i 6000))
      (let ((x (random 320))
            (y (random 200)))
        (pix x y (remainder (* x y (time) 0.001) 60))))))
