#ifndef LIBDB_GLOBAL_H
#define LIBDB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBDB_LIBRARY)
#  define LIBDB_EXPORT Q_DECL_EXPORT
#else
#  define LIBDB_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBDB_GLOBAL_H
