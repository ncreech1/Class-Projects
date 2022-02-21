; Nicholas Creech
; CS365 HW9
; 04/23/21

(define swap 
  (lambda (a b) 
	(if (< a b) 
	  (cons a b) 
	  (cons b a))))

(define avg
  (lambda (a)
	(if (null? a)
	  0
	  (/ (car (avgHelper a)) 1.0))))

(define avgHelper 
  (lambda (a)  
	(if (null? (cdr a)) 
	  (list (car a) 2) ; Base case returns initial list pair (startMean, 2)
	  (let ((ravg (avgHelper (cdr a)))) 
		
		; Computes the average using a running average: runningMean = ((currentMean) * (count - 1) + next) / count 
		; Returns list pair (runningMean, count++)
		(list (/ (+ (* (car ravg) (- (cadr ravg) 1)) (car a)) (cadr ravg)) (+ (cadr ravg) 1))))))

; When calling min, parameter 'a' should be the first number in list 'l'
; Ex: (min '(6 8 2 10) 6) returns 2
(define min
  (lambda (l a)
	(cond
	  ((null? l) '())  
	  ((and (null? (cdr l)) (< a (car l))) a)
	  ((null? (cdr l)) (car l))
	  ((< a (car l)) (min (cdr l) a)) ; Recursive call with no further calculations
	  (#t (min (cdr l) (car l)))))) ; Recursive call with no further calculations


