; Nicholas Creech
; CS365 HW9
; 04/28/21
; NOTE: I chose to do the TN Voice Eval (30 pts) in place of questions 6 and 7 (25 pts)

; Splits a list into half of its values; call again on (cdr l) for other half
(define splitHalf 
  (lambda (l)
	(if (or (null? (cdr l)) (null? (cdr (cdr l))))
	  (list (car l))
	  (append (list (car l)) (splitHalf (cdr (cdr l)))))))

; Merges two sorted lists into one sorted list
(define merge
  (lambda (l1 l2)
	(cond ((null? l1) l2)
		  ((null? l2) l1)
		  ((< (car l1) (car l2)) (cons (car l1) (merge (cdr l1) l2)))
		  (else (cons (car l2) (merge (cdr l2) l1)))))) 

; Recursively splits list l and then merges its values to sort
(define mergesort
  (lambda (l)
	(cond ((or (null? l) (null? (cdr l))) l) ; Base case: empty list or single element list
		  (else (let ((l1 (splitHalf l)) (l2 (splitHalf (cdr l)))) ; Split the list into two halves
				  (merge (mergesort l1) (mergesort l2))))))) ; Recursively split and merge the list

; Filters a list based on a predicate
(define filter
  (lambda (pred l)
	(cond ((null? l) l)
		  ((null? (cdr l)) (let ((result (pred (car l)))) ; Base case: single element list
							 (if (equal? result #t) ; Result is the boolean result of passing head of list to pred
							   (list (car l))
							   '())))
		  (else (let ((result (pred (car l))))
				  (if (equal? result #t) ; Result is the boolean result of passing head of list to pred
					
					; Add head of list to filtered list and recursively check the rest of the list
					(append (list (car l)) (filter pred (cdr l)))

					; Do NOT add head of list to filtered list and recursively check the rest of the list
					(filter pred (cdr l))))))))
