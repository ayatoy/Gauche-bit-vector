(use gauche.test)

(test-start "bit-vector")
(use bit-vector)
(test-module 'bit-vector)

(test* "make-bit-vector" <bit-vector>
       (class-of (make-bit-vector 0)))

(test* "make-bit-vector" <bit-vector>
       (class-of (make-bit-vector 3)))

(test* "make-bit-vector" <bit-vector>
       (class-of (make-bit-vector 500)))

(test* "bit-vector" <bit-vector>
       (class-of (bit-vector)))

(test* "bit-vector" <bit-vector>
       (class-of (bit-vector #f #t #f)))

(test* "bit-vector?" #t
       (bit-vector? (bit-vector)))

(test* "bit-vector-length" 0
       (bit-vector-length (make-bit-vector 0)))

(test* "bit-vector-length" 3
       (bit-vector-length (make-bit-vector 3)))

(test* "bit-vector-length" 500
       (bit-vector-length (make-bit-vector 500)))

(test* "bit-vector-builded?" #t
       (bit-vector-builded? (bit-vector)))

(test* "bit-vector-builded?" #t
       (bit-vector-builded? (bit-vector #f #t #f)))

(test* "bit-vector-builded?" #f
       (let1 bv (bit-vector #f #t #f)
         (bit-vector-set! bv 0 #t)
         (bit-vector-builded? bv)))

(test* "bit-vector-builded?" #t
       (let1 bv (bit-vector #f #t #f)
         (bit-vector-set! bv 0 #t)
         (bit-vector-rank-all bv)
         (bit-vector-builded? bv)))

(test* "bit-vector-clear!" '()
       (bit-vector->list (bit-vector-clear! (bit-vector))))

(test* "bit-vector-clear!" '(#f #f #f)
       (bit-vector->list (bit-vector-clear! (bit-vector #f #t #f))))

(test* "bit-vector-build!" #t
       (let1 bv (bit-vector)
         (and (eq? #t (bit-vector-builded? bv))
              (begin (bit-vector-build! bv)
                     (eq? #t (bit-vector-builded? bv))))))

(test* "bit-vector-build!" #t
       (let1 bv (bit-vector #f #t #f)
         (bit-vector-set! bv 0 #t)
         (and (eq? #f (bit-vector-builded? bv))
              (begin (bit-vector-build! bv)
                     (eq? #t (bit-vector-builded? bv))))))

(test* "bit-vector-ref" #f
       (bit-vector-ref (bit-vector #f #t #f) 0))

(test* "bit-vector-ref" #t
       (bit-vector-ref (bit-vector #f #t #f) 1))

(test* "bit-vector-ref" #t
       (let1 bv (make-bit-vector 500 #f)
         (dotimes [i 500]
           (when (= 0 (modulo i 3))
             (bit-vector-set! bv i #t)))
         (let loop ([x #t] [i 0])
           (if (and x (< i 500))
             (loop (if (= 0 (modulo i 3))
                     (bit-vector-ref bv i)
                     #t)
                   (+ i 1))
             x))))

(test* "bit-vector-ref" (test-error <error>)
       (bit-vector-ref (bit-vector) 0))

(test* "bit-vector-ref" (test-error <error>)
       (bit-vector-ref (bit-vector #f #t #f) -1))

(test* "bit-vector-ref" (test-error <error>)
       (bit-vector-ref (bit-vector #f #t #f) 3))

(test* "bit-vector-set!" #f
       (let1 bv (bit-vector #f #t #f)
         (bit-vector-set! bv 1 #f)
         (bit-vector-ref bv 1)))

(test* "bit-vector-set!" #t
       (let1 bv (bit-vector #f #t #f)
         (bit-vector-set! bv 0 #t)
         (bit-vector-ref bv 0)))

(test* "bit-vector-ref" 167
       (let1 bv (make-bit-vector 500 #f)
         (dotimes [i 500]
           (when (= 0 (modulo i 3))
             (bit-vector-set! bv i #t)))
         (bit-vector-rank-all bv)))

(test* "bit-vector-set!" (test-error <error>)
       (bit-vector-set! (bit-vector) 0 #t))

(test* "bit-vector-set!" (test-error <error>)
       (bit-vector-set! (bit-vector #f #t #f) -1 #t))

(test* "bit-vector-set!" (test-error <error>)
       (bit-vector-set! (bit-vector #f #t #f) 3 #t))

(test* "bit-vector-fill!" '(#t #t #f)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t 0 2)
         (bit-vector->list bv)))

(test* "bit-vector-fill!" '(#f #t #t)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t 1 3)
         (bit-vector->list bv)))

(test* "bit-vector-fill!" '(#t #t #t)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t 0 3)
         (bit-vector->list bv)))

(test* "bit-vector-fill!" '(#t #t #t)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t 0 -1)
         (bit-vector->list bv)))

(test* "bit-vector-fill!" (test-error <error>)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t 0 4)
         (bit-vector->list bv)))

(test* "bit-vector-fill!" (test-error <error>)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t -1 3)
         (bit-vector->list bv)))

(test* "bit-vector-fill!" (test-error <error>)
       (let1 bv (make-bit-vector 3 #f)
         (bit-vector-fill! bv #t 3 2)
         (bit-vector->list bv)))

(test* "bit-vector-rank" 0
       (bit-vector-rank (bit-vector) 0))

(test* "bit-vector-rank" 0
       (bit-vector-rank (bit-vector #f #t #f) 0))

(test* "bit-vector-rank" 0
       (bit-vector-rank (bit-vector #f #t #f) 1))

(test* "bit-vector-rank" 1
       (bit-vector-rank (bit-vector #f #t #f) 2))

(test* "bit-vector-rank" 1
       (bit-vector-rank (bit-vector #f #t #f) 3))

(test* "bit-vector-rank" (test-error <error>)
       (bit-vector-rank (bit-vector) 1))

(test* "bit-vector-rank" (test-error <error>)
       (bit-vector-rank (bit-vector) -1))

(test* "bit-vector-rank" (test-error <error>)
       (bit-vector-rank (bit-vector #f #t #f) 4))

(test* "bit-vector-rank" (test-error <error>)
       (bit-vector-rank (bit-vector #f #t #f) -1))

(test* "bit-vector-rank-all" 0
       (bit-vector-rank-all (bit-vector)))

(test* "bit-vector-rank-all" 1
       (bit-vector-rank-all (bit-vector #f #t #f)))

(test* "bit-vector-select" -1
       (bit-vector-select (bit-vector) 0))

(test* "bit-vector-select" -1
       (bit-vector-select (bit-vector) 1))

(test* "bit-vector-select" 1
       (bit-vector-select (bit-vector #f #t #f) 0))

(test* "bit-vector-select" -1
       (bit-vector-select (bit-vector #f #t #f) 1))

(test* "bit-vector-select" (test-error <error>)
       (bit-vector-select (bit-vector) -1))

(test* "bit-vector-select" (test-error <error>)
       (bit-vector-select (bit-vector #f #t #f) -1))

(test* "bit-vector->integer" 0
       (bit-vector->integer (bit-vector)))

(test* "bit-vector->integer" 2
       (bit-vector->integer (bit-vector #f #t #f)))

(test* "integer->bit-vector" '()
       (bit-vector->list (integer->bit-vector 0)))

(test* "integer->bit-vector" '(#f #t)
       (bit-vector->list (integer->bit-vector 2)))

(test* "bit-vector->list" '()
       (bit-vector->list (bit-vector)))

(test* "bit-vector->list" '(#f #t #f)
       (bit-vector->list (bit-vector #f #t #f)))

(test* "list->bit-vector" '()
       (bit-vector->list (list->bit-vector '())))

(test* "list->bit-vector" '(#f #t #f)
       (bit-vector->list (list->bit-vector '(#f #t #f))))

(test-end :exit-on-failure #t)
