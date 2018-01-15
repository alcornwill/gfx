
#include <Python.h>
#include "gfx.h"

static PyObject *py_init   = NULL;
static PyObject *py_update = NULL;
static PyObject *py_close  = NULL;

static int my_init() {
    PyObject *res = NULL;
    res = PyObject_CallObject(py_init, NULL);
    if (!res)
        return 0;
    Py_DECREF(res);
    return 1;
}

static int my_update() {
    PyObject *res = NULL;
    res = PyObject_CallObject(py_update, NULL);
    if (!res)
        return 0;
    Py_DECREF(res);
    return 1;
}

static int my_close() {
    PyObject *res = NULL;
    res = PyObject_CallObject(py_close, NULL);
    if (!res)
        return 0;
    Py_DECREF(res);
    
    // also do this
    Py_XDECREF(py_init);
    Py_XDECREF(py_update);
    Py_XDECREF(py_close);
    
    return 1;
}


static PyObject *py_gfx_load(PyObject *self, PyObject *args)
{
    // todo get functions from globals?
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
    
    if (!gfx_mainloop())
        return NULL;
    
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

static PyObject *py_gfx_load_spritesheet(PyObject *self, PyObject *args)
{
    char *path = NULL;
    
    if (!PyArg_ParseTuple(args, "s", &path))
        return NULL;
    
    gfx_load_spritesheet(path);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_read_map(PyObject *self, PyObject *args)
{
    char *path = NULL;
    
    if (!PyArg_ParseTuple(args, "s", &path))
        return NULL;
    
    int res = gfx_read_map(path);
    
    return Py_BuildValue("i", res);
}

static PyObject *py_gfx_write_map(PyObject *self, PyObject *args)
{
    char *path = NULL;
    
    if (!PyArg_ParseTuple(args, "s", &path))
        return NULL;
    
    int res = gfx_write_map(path);
    
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

static PyObject *py_gfx_clear(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    
    gfx_clear();
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_set_layer(PyObject *self, PyObject *args)
{
    int layer = 0;
    
    if (!PyArg_ParseTuple(args, "i", &layer))
        return NULL;
    
    gfx_set_layer(layer);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_set_color(PyObject *self, PyObject *args)
{
    int color = 0;
    
    if (!PyArg_ParseTuple(args, "i", &color))
        return NULL;
 
    gfx_set_color(color);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_set_key(PyObject *self, PyObject *args)
{
    int key = 0;
    
    if (!PyArg_ParseTuple(args, "i", &key))
        return NULL;
    
    gfx_set_key(key);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_get_cam(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    
    int x = 0;
    int y = 0;
    
    gfx_get_cam(&x, &y);
    
    return Py_BuildValue("ii", x, y);
}

static PyObject *py_gfx_set_cam(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;
    
    gfx_set_cam(x, y);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_rect(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    
    if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h))
        return NULL;
    
    gfx_draw_rect(x, y, w, h);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_rect_fill(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    
    if (!PyArg_ParseTuple(args, "iiii", &x, &y, &w, &h))
        return NULL;
    
    gfx_draw_rect_fill(x, y, w, h);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_line(PyObject *self, PyObject *args)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    
    if (!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2))
        return NULL;
    
    gfx_draw_line(x1, y1, x2, y2);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_point(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    
    if (!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;
    
    gfx_draw_point(x, y);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_circ(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    int radius = 0;
    
    if (!PyArg_ParseTuple(args, "iii", &x, &y, &radius))
        return NULL;
    
    gfx_draw_circ(x, y, radius);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_circ_fill(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    int radius = 0;
    
    if (!PyArg_ParseTuple(args, "iii", &x, &y, &radius))
        return NULL;
    
    gfx_draw_circ_fill(x, y, radius);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_text_at(PyObject *self, PyObject *args)
{
    int x = 0;
    int y = 0;
    char *text = NULL;
    
    if (!PyArg_ParseTuple(args, "iis", &x, &y, &text))
        return NULL;
    
    gfx_draw_text_at(x, y, text);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_sprite(PyObject *self, PyObject *args)
{
    int index = 0;
    int x = 0;
    int y = 0;
    float r = 0.0f;
    int flags = 0;
    
    if (!PyArg_ParseTuple(args, "iiifi", &index, &x, &y, &r, &flags))
        return NULL;
    
    gfx_draw_sprite(index, x, y, r, flags);
    
    Py_RETURN_NONE;
}

static PyObject *py_gfx_draw_map(PyObject *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, ""))
        return NULL;
    
    gfx_draw_map();
    
    Py_RETURN_NONE;
}


static PyMethodDef gfx_methods[] = {
    {"init",          py_gfx_load,          METH_VARARGS, ""},
    {"mainloop",      py_gfx_mainloop,      METH_VARARGS, ""},
    {"key",           py_gfx_get_key,       METH_VARARGS, ""},
    {"keydown",       py_gfx_get_keydown,   METH_VARARGS, ""},
    {"keyup",         py_gfx_get_keyup,     METH_VARARGS, ""},
    {"load_wav",      py_gfx_load_wav,      METH_VARARGS, ""},
    {"play_wav",      py_gfx_play_wav,      METH_VARARGS, ""},
    {"load_spritesheet",py_gfx_load_spritesheet,METH_VARARGS, ""},
    {"read_map",      py_gfx_read_map,      METH_VARARGS, ""},
    {"write_map",     py_gfx_write_map,     METH_VARARGS, ""},
    {"map_data",      py_gfx_map_data,      METH_VARARGS, ""},
    {"map_flags",     py_gfx_map_flags,     METH_VARARGS, ""},
    {"set_map",       py_gfx_set_map,       METH_VARARGS, ""},
    {"set_map_flags", py_gfx_set_map_flags, METH_VARARGS, ""},
    {"clear",         py_gfx_clear,         METH_VARARGS, ""},
    {"set_layer",     py_gfx_set_layer,     METH_VARARGS, ""},
    {"color",         py_gfx_set_color,     METH_VARARGS, ""},
    {"ckey",          py_gfx_set_key,       METH_VARARGS, ""},
    // use python property?
    {"set_cam",       py_gfx_set_cam,       METH_VARARGS, ""},
    {"get_cam",       py_gfx_get_cam,       METH_VARARGS, ""},
    {"rect",          py_gfx_draw_rect,     METH_VARARGS, ""},
    {"rectfill",      py_gfx_draw_rect_fill,METH_VARARGS, ""},
    {"line",          py_gfx_draw_line,     METH_VARARGS, ""},
    {"point",         py_gfx_draw_point,    METH_VARARGS, ""},
    {"circ",          py_gfx_draw_circ,     METH_VARARGS, ""},
    {"circfill",      py_gfx_draw_circ_fill,METH_VARARGS, ""},
    {"text",          py_gfx_draw_text_at,  METH_VARARGS, ""},
    {"sprite",        py_gfx_draw_sprite,   METH_VARARGS, ""},
    {"map",           py_gfx_draw_map,      METH_VARARGS, ""},
    {NULL, NULL}
};

PyMODINIT_FUNC initgfx(void)
{
    PyObject *m;
    m = Py_InitModule("gfx", gfx_methods);
    
    // define constants
#define ADD_CONST(x) \
    if (PyModule_AddIntConstant(m, #x, x)) Py_DECREF(m)
    
    ADD_CONST(K_UP);
    ADD_CONST(K_DOWN);
    ADD_CONST(K_LEFT);
    ADD_CONST(K_RIGHT);
    ADD_CONST(K_A);
    ADD_CONST(K_B);
    ADD_CONST(K_START);
    ADD_CONST(K_SELECT);

    ADD_CONST(BLACK);
    ADD_CONST(WHITE);
    ADD_CONST(RED);
    ADD_CONST(CYAN);
    ADD_CONST(VIOLET);
    ADD_CONST(GREEN);
    ADD_CONST(BLUE);
    ADD_CONST(YELLOW);
    ADD_CONST(ORANGE);
    ADD_CONST(BROWN);
    ADD_CONST(PINK);
    ADD_CONST(DK_GREY);
    ADD_CONST(GREY);
    ADD_CONST(LT_GREEN);
    ADD_CONST(LT_BLUE);
    ADD_CONST(LT_GREY);
    
    ADD_CONST(SIZE_SMALL);
    ADD_CONST(SIZE_MEDIUM);
    ADD_CONST(SIZE_BIG);
    ADD_CONST(SIZE_HUGE);
    ADD_CONST(SHAPE_SQUARE);
    ADD_CONST(SHAPE_WIDE);
    ADD_CONST(SHAPE_TALL);
    ADD_CONST(SCALE_X);
    ADD_CONST(SCALE_Y);
    ADD_CONST(FLIP_HORIZONTAL);
    ADD_CONST(FLIP_VERTICAL);
}
