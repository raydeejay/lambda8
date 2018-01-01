(begin 
  (display "loading\n")

  ;; this code belongs in some library
  (define (filter p ls)
    (let ((result (list)))
      (for-each (lambda (n) (when (p n) (set! result (cons n result)))) ls)
      (reverse result)))

  (define (btn n)
    (> (logand KEYS (ash 1 n)) 0))

  (define (btnp n)
    (and (> (logand KEYS (ash 1 n)) 0)
         (= (logand LASTKEYS (ash 1 n)) 0)))

  (define (collideAABB a b)
    (let ((X1 ((a 'x)))
          (W1 ((a 'w)))
          (Y1 ((a 'y)))
          (H1 ((a 'h)))
          (X2 ((b 'x)))
          (W2 ((b 'w)))
          (Y2 ((b 'y)))
          (H2 ((b 'h))))
      (not (or (< (+ X1 W1) X2)
               (< (+ X2 W2) X1)
               (< (+ Y1 H1) Y2)
               (< (+ Y2 H2) Y1)))))

  (define (collide-with-mask a b)
    (let ((X1 ((a 'x)))
          (W1 ((a 'w)))
          (Y1 ((a 'y)))
          (H1 ((a 'h)))
          (X2 ((b 'x)))
          (W2 ((b 'w)))
          (Y2 ((b 'y)))
          (H2 ((b 'h))))
      (not (or (< (+ X1 W1) X2)
               (< (+ X2 W2) X1)
               (< (+ Y1 H1) Y2)
               (< (+ Y2 H2) Y1)))))

  (define (make-sprite filename-or-id x y w h)
    (let ((id -1) (dx 0) (dy 0))
      (set! id (if (number? filename-or-id)
                   filename-or-id
                   (define-sprite filename-or-id)))
      (lambda (method-name)
        (case method-name
          ('spr! (lambda (value) (set! id value)))
          ('spr  (lambda () id))

          ('x! (lambda (value) (set! x value)))
          ('x  (lambda () x))
          ('y! (lambda (value) (set! y value)))
          ('y  (lambda () y))

          ('w! (lambda (value) (set! w value)))
          ('w  (lambda () w))
          ('h! (lambda (value) (set! h value)))
          ('h  (lambda () h))

          ('dx! (lambda (value) (set! dx value)))
          ('dx  (lambda () dx))
          ('dy! (lambda (value) (set! dy value)))
          ('dy  (lambda () dy))

          ('update (lambda ()
                     (set! x (+ x dx))
                     (set! y (+ y dy))))
          ('draw (lambda ()
                   (spr id x y w h)))))))

  (define (make-road)
    (let ((image (make-sprite "road.png" 0 0 64 128))
          (step 0))
      (lambda (method-name)
        (case method-name
          ('update (lambda ()
                     (set! step (+ step 1))
                     (if (> step ((image 'w)))
                         (set! step 0))))
          ('draw (lambda ()
                   (for-each (lambda (n)
                               ((image 'x!) (- (* ((image 'w)) n) step))
                               ((image 'draw)))
                             (list 0 1 2 3 4 5))))))))

;;; game proper begins here
  ;; define a couple of sprites
  (define star (make-sprite "star.png" 10 76 16 16))
  (define moon (make-sprite "moon.png" 50 30 16 16))

  (define cars (list))

  (define bg (make-road))

  ;; and a couple of sounds
  (define beep-sfx (define-sfx "beep1.wav"))
  (define fail-sfx (define-sfx "fail.wav"))

  ;; the hook/callback
  (define update
    (let ((tick 0) (score 0))
      (lambda ()
        ;; spawn a car
        (when (btnp 5)
              (let ((vehicle (make-sprite ((moon 'spr))
                                          300 (+ 30 (* 20 (random 3))) 16 16)))
                (sfx  beep-sfx)
                ((vehicle 'dx!) -2)
                ((vehicle 'y))
                (set! cars (cons vehicle cars))))

        ;; bounce the moon
        (if (or (< ((moon 'x)) 0)
                (> ((moon 'x)) SCREEN-WIDTH))
            ((moon 'dx!) (- 0 ((moon 'dx)))))

        ;; move the star
        (if (btn 0)
            ((star 'y!) (- ((star 'y)) 1)))
        (if (btn 1)
            ((star 'y!) (+ ((star 'y)) 1)))
        (if (btn 2)
            ((star 'x!) (- ((star 'x)) 1)))
        (if (btn 3)
            ((star 'x!) (+ ((star 'x)) 1)))

        ;; draw the cars
        (map (lambda (c) ((c 'update))) cars)

        ;; remove the cars outside the screen from the list
        ;; should destroy them too
        (set! cars (filter (lambda (c)
                             (if (> ((c 'x)) -100)
                                 #t
                                 (begin
                                        ;((c 'destroy))
                                   (display "I should be destroyed now\n")
                                   #f)))
                           cars))

        ;; draw player
        (let ((reset (lambda ()
                       ((star 'y!) 76)
                       ((star 'dy!) 0))))
          (if (< ((star 'y)) -100)
              (reset)
              ((star 'dy!) (* 1.1 ((star 'dy)))))
          ((star 'update))

          ;; collision
          (map (lambda (c)
                 (when (collideAABB star c)
                       (begin (sfx fail-sfx)
                              (set! score (- score 100))
                              (reset))))
               cars))

        ;; move the road
        ((bg 'update))

        ;; render
        ;;(cls 1)
        ((bg 'draw))
        (map (lambda (m) ((m 'draw))) cars)
        ((star 'draw))

        ;; end cycle
        (set! score (+ score 100))
        (let ((sc (format "~d" score)))
          (printxy sc (- SCREEN-WIDTH (* 10 (length sc))) 0))
        (set! tick (+ tick 1)))))

  (display "loaded!\n")

  #t)
