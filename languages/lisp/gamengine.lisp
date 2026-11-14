
(defpackage :gamengine
  (:use :cl)
  (:export :create-object
           :list-objects
           :engine-version))

(in-package :gamengine)

(defparameter *engine-version* "0.1.0")
(defparameter *objects* nil)

(defun engine-version ()
  *engine-version*)

(defun create-object (name)
  (let ((object
          (list :name name
                :position '(0.0 0.0 0.0))))
    (push object *objects*)
    object))

(defun list-objects ()
  (reverse *objects*))

(defun reset-scene ()
  (setf *objects* nil))
