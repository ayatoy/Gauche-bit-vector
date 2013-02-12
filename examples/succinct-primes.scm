(define-module succinct-primes
  (use gauche.record)
  (use math.prime)
  (use bit-vector)
  (export <succinct-primes>
          succinct-primes?
          make-succinct-primes
          succinct-primes-prime?
          succinct-primes-ref
          succinct-primes-count))

(select-module succinct-primes)

(define-record-type <succinct-primes> %make-succinct-primes succinct-primes?
  (index succinct-primes-index))

(define (make-succinct-primes n)
  (let1 bv (make-bit-vector n #f)
    (let loop ([primes *primes*])
      (let1 p (car primes)
        (if (< p n)
          (begin
            (bit-vector-set! bv p #t)
            (loop (cdr primes)))
          (begin
            (bit-vector-build! bv)
            (%make-succinct-primes bv)))))))

(define (succinct-primes-prime? sp n)
  (bit-vector-ref (succinct-primes-index sp) n))

(define (succinct-primes-ref sp i)
  (bit-vector-select (succinct-primes-index sp) i))

(define (succinct-primes-count sp start end)
  (let1 index (succinct-primes-index sp)
    (- (bit-vector-rank index end) (bit-vector-rank index start))))
