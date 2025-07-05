/*
 * static_model.c
 *
 * automatically generated from simple3.icd
 */
#include "static_model.h"

static void initializeValues();



LogicalDevice iedModel_LD0 = {
    LogicalDeviceModelType,
    "LD0",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_LD0_LLN00,
    NULL
};

LogicalNode iedModel_LD0_LLN00 = {
    LogicalNodeModelType,
    "LLN00",
    (ModelNode*) &iedModel_LD0,
    NULL,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1,
};

DataObject iedModel_LD0_LLN00_CapacidadLinea1 = {
    DataObjectModelType,
    "CapacidadLinea1",
    (ModelNode*) &iedModel_LD0_LLN00,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1_mag,
    0,
    -1
};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea1_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1_q,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea1_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea1,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN00_CapacidadLinea2 = {
    DataObjectModelType,
    "CapacidadLinea2",
    (ModelNode*) &iedModel_LD0_LLN00,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2_mag,
    0,
    -1
};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea2_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2_q,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea2_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea2_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea2_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea2,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataObject iedModel_LD0_LLN00_CapacidadLinea3 = {
    DataObjectModelType,
    "CapacidadLinea3",
    (ModelNode*) &iedModel_LD0_LLN00,
    NULL,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3_mag,
    0,
    -1
};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea3_mag = {
    DataAttributeModelType,
    "mag",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3_q,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3_mag_f,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea3_mag_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3_mag,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea3_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3,
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3_t,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0,
    NULL,
    0};

DataAttribute iedModel_LD0_LLN00_CapacidadLinea3_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_LD0_LLN00_CapacidadLinea3,
    NULL,
    NULL,
    0,
    -1,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};









IedModel iedModel = {
    "IED1",
    &iedModel_LD0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{
}
