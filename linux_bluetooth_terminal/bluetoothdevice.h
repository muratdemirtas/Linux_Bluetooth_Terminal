#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "fcntl.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>

/**
 * @struct bt_dev_info_t used for getting bluetooth device adress with ioctl
 */
typedef struct
{
    QString HW_Name;    /** bluetooth adapter Hardware name */
    QString HW_Address; /** bluetooth adapter Hardware address */

}bt_dev_info_t;

/**
  * @struct bt_op_status_t
  */
typedef enum
{
    BTO_SUCCESS,
    BTO_ERROR
}bt_op_status_t;

class bluetoothDevice : public QObject
{
    Q_OBJECT
public:
    explicit bluetoothDevice(QObject *parent = nullptr);

    /**
     * @brief startBluetoothDevices
     * @param hardware_id
     * @return
     */
    QList<QString> scanBluetoothDevices(QString hardware_id);


    bt_op_status_t findBluetoothAdaptors();

signals:

public slots:


private:
    QList<bt_dev_info_t> m_bluetoothAdapters;
};

#endif // BLUETOOTHDEVICE_H
