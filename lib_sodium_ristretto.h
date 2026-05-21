#ifndef MRP_01_LIB_SODIUM_RISTRETTO_H
#define MRP_01_LIB_SODIUM_RISTRETTO_H

my_bool ristrettoisvalidpoint_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message );
long long ristrettoisvalidpoint( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* is_null, char* error );
my_bool ristrettorandom_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
void ristrettorandom_deinit( UDF_INIT* initid );
char* ristrettorandom( const UDF_INIT* initid, UDF_ARGS* args, char* result,
                       unsigned long* length, char* is_null,
                       char* error );
my_bool ristrettofromhash_init( UDF_INIT* initid, const UDF_ARGS* args,
                                char* message );
void ristrettofromhash_deinit( UDF_INIT* initid );
char* ristrettofromhash( const UDF_INIT* initid, const UDF_ARGS* args,
                         char* result,
                         unsigned long* length, char* is_null,
                         char* error );
my_bool scalarmultristretto_init( UDF_INIT* initid, const UDF_ARGS* args,
                                  char* message );
void scalarmultristretto_deinit( UDF_INIT* initid );
char* scalarmultristretto( const UDF_INIT* initid, const UDF_ARGS* args,
                           char* result,
                           unsigned long* length, char* is_null,
                           char* error );
my_bool scalarmultristrettobase_init( UDF_INIT* initid, const UDF_ARGS* args,
                                      char* message );
void scalarmultristrettobase_deinit( UDF_INIT* initid );
char* scalarmultristrettobase( const UDF_INIT* initid, const UDF_ARGS* args,
                               char* result,
                               unsigned long* length, char* is_null,
                               char* error );
my_bool ristrettoadd_init( UDF_INIT* initid, const UDF_ARGS* args,
                           char* message );
void ristrettoadd_deinit( UDF_INIT* initid );
char* ristrettoadd( const UDF_INIT* initid, const UDF_ARGS* args, char* result,
                    unsigned long* length, char* is_null, char* error );
my_bool ristrettosub_init( UDF_INIT* initid, const UDF_ARGS* args,
                           char* message );
void ristrettosub_deinit( UDF_INIT* initid );
char* ristrettosub( const UDF_INIT* initid, const UDF_ARGS* args, char* result,
                    unsigned long* length, char* is_null, char* error );
my_bool ristrettoscalarrandom_init( UDF_INIT* initid, UDF_ARGS* args,
                                    char* message );
void ristrettoscalarrandom_deinit( UDF_INIT* initid );
char* ristrettoscalarrandom( const UDF_INIT* initid, UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null,
                             char* error );
my_bool ristrettoscalarreduce_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message );
void ristrettoscalarreduce_deinit( UDF_INIT* initid );
char* ristrettoscalarreduce( const UDF_INIT* initid, const UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null,
                             char* error );
my_bool ristrettoscalarinvert_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message );
void ristrettoscalarinvert_deinit( UDF_INIT* initid );
char* ristrettoscalarinvert( const UDF_INIT* initid, const UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null,
                             char* error );
my_bool ristrettoscalarnegate_init( UDF_INIT* initid, const UDF_ARGS* args,
                                    char* message );
void ristrettoscalarnegate_deinit( UDF_INIT* initid );
char* ristrettoscalarnegate( const UDF_INIT* initid, const UDF_ARGS* args,
                             char* result,
                             unsigned long* length, char* is_null,
                             char* error );
my_bool ristrettoscalarcomplement_init( UDF_INIT* initid, const UDF_ARGS* args,
                                        char* message );
void ristrettoscalarcomplement_deinit( UDF_INIT* initid );
char* ristrettoscalarcomplement( const UDF_INIT* initid, const UDF_ARGS* args,
                                 char* result,
                                 unsigned long* length, char* is_null,
                                 char* error );
my_bool ristrettoscalaradd_init( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* message );
void ristrettoscalaradd_deinit( UDF_INIT* initid );
char* ristrettoscalaradd( const UDF_INIT* initid, const UDF_ARGS* args,
                          char* result,
                          unsigned long* length, char* is_null,
                          char* error );
my_bool ristrettoscalarsub_init( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* message );
void ristrettoscalarsub_deinit( UDF_INIT* initid );
char* ristrettoscalarsub( const UDF_INIT* initid, const UDF_ARGS* args,
                          char* result,
                          unsigned long* length, char* is_null,
                          char* error );
my_bool ristrettoscalarmul_init( UDF_INIT* initid, const UDF_ARGS* args,
                                 char* message );
void ristrettoscalarmul_deinit( UDF_INIT* initid );
char* ristrettoscalarmul( const UDF_INIT* initid, const UDF_ARGS* args,
                          char* result,
                          unsigned long* length, char* is_null,
                          char* error );
#endif //MRP_01_LIB_SODIUM_RISTRETTO_H