#ifndef PRINTERSYSTEM_H
#define PRINTERSYSTEM_H

#include <QObject>


/**
 *
 */
enum
{
   DEBUG_ERROR = 0x0001,
   DEBUG_INFO  = 0x0002,
}debug_types_t;

/**
 * @brief The printerSystem class
 */
class printerSystem : public QObject
{
    Q_OBJECT
public:
    explicit printerSystem(QObject *parent = nullptr);
    /**
     * @brief setDebugTypes
     * @param debug_types
     */
    void setDebugTypes(uint32_t debug_types);


signals:

public slots:
};

#endif // PRINTERSYSTEM_H
