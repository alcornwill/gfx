
#include <Python.h>
#include "gfx.h"

static PyObject *testy(PyObject *self, PyObject *args)
{
    printf("Hello, world\n");
    Py_RETURN_NONE;
}

static PyMethodDef gfx_methods[] = {
    {"test", testy, METH_VARARGS, "simple hello world"},
    {NULL, NULL}
};

PyMODINIT_FUNC initgfx(void)
{
    Py_InitModule("gfx", gfx_methods);
}