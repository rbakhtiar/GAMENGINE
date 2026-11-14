
(ql:quickload :cffi)

(defpackage :gamengine-bridge
  (:use :cl :cffi)
  (:export :create-engine
           :destroy-engine
           :create-object
           :object-count
           :set-position
           :remove-object))

(in-package :gamengine-bridge)

(define-foreign-library gamengine-core
  (:unix (:or "libgamengine_core.so"
              "libgamengine_core.dylib"))
  (:windows "gamengine_core.dll"))

(use-foreign-library gamengine-core)

(defcfun ("ge_engine_create" %engine-create) :pointer)
(defcfun ("ge_engine_destroy" %engine-destroy) :void
  (engine :pointer))

(defcfun ("ge_object_create" %object-create) :uint64
  (engine :pointer)
  (name :string))

(defcfun ("ge_object_count" %object-count) :size
  (engine :pointer))

(defcfun ("ge_object_set_position" %set-position) :int
  (engine :pointer)
  (id :uint64)
  (x :float)
  (y :float)
  (z :float))

(defcfun ("ge_object_remove" %object-remove) :int
  (engine :pointer)
  (id :uint64))

(defun create-engine ()
  (%engine-create))

(defun destroy-engine (engine)
  (%engine-destroy engine))

(defun create-object (engine name)
  (%object-create engine name))

(defun object-count (engine)
  (%object-count engine))

(defun set-position (engine id x y z)
  (= 1 (%set-position engine id x y z)))

(defun remove-object (engine id)
  (= 1 (%object-remove engine id)))
