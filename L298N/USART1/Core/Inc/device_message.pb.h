/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.8 */

#ifndef PB_API_COLLECTION_API_COLLECTION_DEVICE_MESSAGE_PB_H_INCLUDED
#define PB_API_COLLECTION_API_COLLECTION_DEVICE_MESSAGE_PB_H_INCLUDED
#include "pb.h"

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Struct definitions */
typedef struct _api_collection_GeoPoint {
    double lat; /* @gotags: json:"lat" */
    double lon; /* @gotags: json:"lon" */
} api_collection_GeoPoint;

typedef struct _api_collection_CollectionData {
    int64_t timestamp; /* @gotags: json:"timestamp" */
    bool has_geo_point;
    api_collection_GeoPoint geo_point; /* @gotags: json:"geoPoint" */
    float temperature; /* @gotags: json:"temperature" */
} api_collection_CollectionData;


#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define api_collection_GeoPoint_init_default     {0, 0}
#define api_collection_CollectionData_init_default {0, false, api_collection_GeoPoint_init_default, 0}
#define api_collection_GeoPoint_init_zero        {0, 0}
#define api_collection_CollectionData_init_zero  {0, false, api_collection_GeoPoint_init_zero, 0}

/* Field tags (for use in manual encoding/decoding) */
#define api_collection_GeoPoint_lat_tag          1
#define api_collection_GeoPoint_lon_tag          2
#define api_collection_CollectionData_timestamp_tag 1
#define api_collection_CollectionData_geo_point_tag 2
#define api_collection_CollectionData_temperature_tag 3

/* Struct field encoding specification for nanopb */
#define api_collection_GeoPoint_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, DOUBLE,   lat,               1) \
X(a, STATIC,   SINGULAR, DOUBLE,   lon,               2)
#define api_collection_GeoPoint_CALLBACK NULL
#define api_collection_GeoPoint_DEFAULT NULL

#define api_collection_CollectionData_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, INT64,    timestamp,         1) \
X(a, STATIC,   OPTIONAL, MESSAGE,  geo_point,         2) \
X(a, STATIC,   SINGULAR, FLOAT,    temperature,       3)
#define api_collection_CollectionData_CALLBACK NULL
#define api_collection_CollectionData_DEFAULT NULL
#define api_collection_CollectionData_geo_point_MSGTYPE api_collection_GeoPoint

extern const pb_msgdesc_t api_collection_GeoPoint_msg;
extern const pb_msgdesc_t api_collection_CollectionData_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define api_collection_GeoPoint_fields &api_collection_GeoPoint_msg
#define api_collection_CollectionData_fields &api_collection_CollectionData_msg

/* Maximum encoded size of messages (where known) */
#define API_COLLECTION_API_COLLECTION_DEVICE_MESSAGE_PB_H_MAX_SIZE api_collection_CollectionData_size
#define api_collection_CollectionData_size       36
#define api_collection_GeoPoint_size             18

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
