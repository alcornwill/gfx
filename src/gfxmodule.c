
#include <Python.h>
#include "gfx.h"

static PyObject *py_init   = NULL;
static PyObject *py_update = NULL;
static PyObject *py_close  = NULL;

static void my_init() {
    PyObject_CallObject(py_init, NULL);
}

static void my_update() {
    PyObject_CallObject(py_update, NULL);
}

static void my_close() {
    PyObject_CallObject(py_close, NULL);
    
    // also do this
    Py_XDECREF(py_init);
    Py_XDECREF(py_update);
    Py_XDECREF(py_close);
}


static PyObject *py_gfx_load(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "OOO", &py_init, &py_update, &py_close)) 
        return NULL;
    
    if (!PyCallable_Check(py_init)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }
    if (!PyCallable_Check(py_update)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }
    if (!PyCallable_Check(py_close)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be callable");
        return NULL;
    }
        
    Py_XINCREF(py_init); 
    Py_XINCREF(py_update);
    Py_XINCREF(py_close); 
    
    
    gfx_load(my_init, my_update, my_close);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_mainloop(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    
    
    gfx_mainloop();
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_get_key(PyObject *self, PyObject *args)
{
    int i = 0;
    
    if (!PyArg_ParseTuple(args, "i", &i))
        return NULL;
    
    int res = gfx_get_key(i);
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_get_keydown(PyObject *self, PyObject *args)
{
    int i = 0;
    
    if (!PyArg_ParseTuple(args, "i", &i))
        return NULL;
    
    int res = gfx_get_keydown(i);
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_get_keyup(PyObject *self, PyObject *args)
{
    int i = 0;
    
    if (!PyArg_ParseTuple(args, "i", &i))
        return NULL;
    
    int res = gfx_get_keyup(i);
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_load_wav(PyObject *self, PyObject *args)
{
    char *path = NULL;
    int index = 0;
    
    if (!PyArg_ParseTuple(args, "si", &path, &index))
        return NULL;
    
    gfx_load_wav(path, index);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_play_wav(PyObject *self, PyObject *args)
{
    int index = 0;
    
    if (!PyArg_ParseTuple(args, "i", &index))
        return NULL;
    
    gfx_play_wav(index);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_read_map(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    
    int res = gfx_read_map();
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_write_map(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    
    int res = gfx_write_map();
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_map_data(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;
    
    int res = gfx_map_data(x, y);
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_map_flags(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;
    
    int res = gfx_map_flags(x, y);
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_set_map(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    int value = 0;
    
    if (!PyArg_ParseTuple(args, "iii", &x, &y, &value))
        return NULL;
    
    gfx_set_map(x, y, value);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_set_map_flags(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    int value = 0;
    
    if (!PyArg_ParseTuple(args, "iii", &x, &y, &value))
        return NULL;
    
    gfx_set_map_flags(x, y, value);
    
    Py_RETURN_NONE;
}



static PyMethodDef gfx_methods[] = {
    {"init",          py_gfx_load,          METH_VARARGS, ""},
    {"mainloop",      py_gfx_mainloop,      METH_VARARGS, ""},
    {"key",           py_gfx_get_key ,      METH_VARARGS, ""},
    {"keydown",       py_gfx_get_keydown,   METH_VARARGS, ""},
    {"keyup",         py_gfx_get_keyup,     METH_VARARGS, ""},
    {"load_wav",      py_gfx_load_wav,      METH_VARARGS, ""},
    {"play_wav",      py_gfx_play_wav,      METH_VARARGS, ""},
    {"read_map",      py_gfx_read_map,      METH_VARARGS, ""},
    {"write_map",     py_gfx_write_map,     METH_VARARGS, ""},
    {"map_data",      py_gfx_map_data,      METH_VARARGS, ""},
    {"map_flags",     py_gfx_map_flags,     METH_VARARGS, ""},
    {"set_map",       py_gfx_set_map,       METH_VARARGS, ""},
    {"set_map_flags", py_gfx_set_map_flags, METH_VARARGS, ""},
    {NULL, NULL}
};

PyMODINIT_FUNC initgfx(void)
{
    Py_InitModule("gfx", gfx_methods);
}