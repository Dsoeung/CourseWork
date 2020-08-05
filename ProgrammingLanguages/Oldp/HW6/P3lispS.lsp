;(defmacro +=(start add)
;(+= numericVariable incrementValue)
;> (setf x 1)
;1
;> (+= x 5)
;6
;

(defmacro += (start &optional (num 1))
  `(setf ,start (+ ,num ,start))
)

;; iterate
;; Model after
;;(iterate controlVariable beginValueExpr endValueExpr incrExpr bodyexpr1 bodyexpr2 ... bodyexprN) 
;;we use rest to express the values bodyexpr1 bodyexpr2 ... bodyexprN
(defmacro iterate (controlVariable beginValueExpr endValueExpr incrExpr &rest bodyExpr1)
    (let ((iter(gensym))(end(gensym)) )
        `(do ((,iter ,incrExpr)(,end ,endValueExpr)(,controlVariable ,beginValueExpr(+= ,controlVariable ,iter)))
            ((> ,controlVariable ,end) T)
            ,@ bodyExpr1
        )
    )
)