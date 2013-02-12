(define-module bit-vector
  (export <bit-vector>
          make-bit-vector
          bit-vector
          bit-vector?
          bit-vector-length
          bit-vector-builded?
          bit-vector-clear!
          bit-vector-build!
          bit-vector-ref
          bit-vector-set!
          bit-vector-fill!
          bit-vector-rank
          bit-vector-rank-all
          bit-vector-select
          bit-vector->integer
          integer->bit-vector
          bit-vector->list
          list->bit-vector
          bit-vector->string
          string->bit-vector))

(select-module bit-vector)

(dynamic-load "bit_vector")

(define (bit-vector . args)
  (list->bit-vector args))

(define (bit-vector->integer bv)
  (let1 len (bit-vector-length bv)
    (let loop ([i 0] [int 0])
      (if (< i len)
        (loop (+ i 1)
              (if (bit-vector-ref bv i)
                (copy-bit i int #t)
                int))
        int))))

(define (integer->bit-vector int)
  (let1 len (integer-length int)
    (rlet1 bv (make-bit-vector len #f)
      (dotimes [i len]
        (when (logbit? i int)
          (bit-vector-set! bv i #t)))
      (bit-vector-build! bv))))

(define (bit-vector->list bv)
  (let1 len (bit-vector-length bv)
    (rlet1 lst (make-list len #f)
      (let loop ([i 0] [lst lst])
        (when (< i len)
          (when (bit-vector-ref bv i)
            (set-car! lst #t))
          (loop (+ i 1) (cdr lst)))))))

(define (list->bit-vector lst)
  (let1 len (length lst)
    (rlet1 bv (make-bit-vector len #f)
      (let loop ([i 0] [lst lst])
        (when (< i len)
          (when (car lst)
            (bit-vector-set! bv i #t))
          (loop (+ i 1) (cdr lst))))
      (bit-vector-build! bv))))

(define (bit-vector->string bv)
  (let1 len (bit-vector-length bv)
    (rlet1 str (make-string len #\0)
      (dotimes [i len]
        (when (bit-vector-ref bv i)
          (string-set! str i #\1))))))

(define (string->bit-vector str)
  (let1 len (string-length str)
    (rlet1 bv (make-bit-vector len #f)
      (dotimes [i len]
        (when (char=? #\1 (string-ref str i))
          (bit-vector-set! bv i #t))))))
