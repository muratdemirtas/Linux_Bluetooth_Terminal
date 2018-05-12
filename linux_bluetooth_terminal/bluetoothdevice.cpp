/**
  * @todo .c files must be moved to another .c files
  *
  */

#include "bluetoothdevice.h" /* Application bluetoothDevice Class*/

#define for_each_opt(opt, long, short) while ((opt=getopt_long(argc, argv, short ? short:"+", long, NULL)) != -1)
#define MAX_ADDRESS_PTR_SIZE 18 /** for 00:00:00:00:00:00 conversions */


QList<bt_dev_info_t> current_hardware_list; /* used for storing hardware info*/

/**
 * @brief dev_info get hardware info using ioctl
 * @param s file descriptor id
 * @param dev_id current hardware id if found any bluetooth device
 * @param arg not used
 * @return return 0, othervise program will corrupt
 */
static int dev_info(int s, int dev_id, long arg)
{
    struct hci_dev_info di = { .dev_id = dev_id };
    char addr[MAX_ADDRESS_PTR_SIZE];  /*create string for converting addr to string*/

    /* try ioctl call for device info from linux BT stack driver*/
    if (ioctl(s, HCIGETDEVINFO, (void *) &di))
        return 0;

    ba2str(&di.bdaddr, addr); /* converting address to string*/

    /* adding device informations to current_hardware_list*/
    bt_dev_info_t bt_dev_info;
    bt_dev_info.HW_Name    = (char*)di.name;
    bt_dev_info.HW_Address = (char*)addr;

    /* append device info to QList*/
    current_hardware_list.append(bt_dev_info);

    return 0; /* return*/
}


/**
 * @brief bluetoothDevice::bluetoothDevice Constructor define
 * @param parent
 */
bluetoothDevice::bluetoothDevice(QObject *parent) : QObject(parent)
{
    findBluetoothAdaptors();
    scanBluetoothDevices("");
}

/**
 * @brief bluetoothDevice::findBluetoothAdaptors
 * @return
 */
bt_op_status_t bluetoothDevice::findBluetoothAdaptors()
{

    hci_for_each_dev(HCI_UP, dev_info, 0);

    if(current_hardware_list.length() < 0 )
    {
        perror("Can't found any BT HCI device on this system!\r\n");
        return BTO_ERROR;
    }
    else if (current_hardware_list.length() > 1)
    {
        for(int i  = 0 ; i < current_hardware_list.length() ; i++)
            printf("Bulunan Aygit %s, Address %s\r\n", current_hardware_list.at(i).
               HW_Address.toStdString().c_str(),
                        current_hardware_list.at(i).HW_Name.toStdString().c_str());
    }


   m_bluetoothAdapters = current_hardware_list;

   return BTO_SUCCESS;
}


/**
 * @brief bluetoothDevice::scanBluetoothDevices
 * @param hardware_id
 * @return
 */
QList<QString> bluetoothDevice::scanBluetoothDevices(QString hardware_id)
{
     inquiry_info *inquirys = NULL;
     int max_rsp = 255, num_rsp;
     int dev_id, socket, len =8, flags = IREQ_CACHE_FLUSH;
     char addr[19] = { 0 };
     char name[248] = { 0 };

     QList<QString> BTDevices;  /* return BT list for function call*/

     /* get device Id and use this as primary BT adapter */
     dev_id = hci_get_route(NULL);

     /* get device id and try to open*/
     socket = hci_open_dev( dev_id );

     /* if socket can't open or error occurred*/
     if (dev_id < 0 || socket < 0) {

         /*print err message to standart error output and exit*/
         perror("Cannot open BT HCI device socket, device busy?\r\n");
         return BTDevices;
     }

     /* allocate memory remote for getting bluetooth device informations from driver*/
     inquirys = (inquiry_info*)malloc(max_rsp * sizeof(inquirys));

     /* get maximum remote BT devices and get informations using inquirys struct */
     num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &inquirys, flags);

     /* if return value <0 , then must be scan error*/
     if( num_rsp < 0 )
            perror("An an error occurred while scanning BT HCI devices\r\n");

     /* if num_rsp = 0 , then any device is not in BT range*/
     else if(num_rsp == 0 )
            perror("Can't find any BT devices after scanning\r\n");

     /* if num_rsp > 1, then devices founded */
     else
     {
         for (int i = 0; i < num_rsp; i++) {
             ba2str(&(inquirys+i)->bdaddr, addr);
             memset(name, 0, sizeof(name));

             /* if name can resolve */
             if (hci_read_remote_name(socket, &(inquirys+i)->bdaddr, sizeof(name),
                 name, 0) < 0)
             strcpy(name, "[unknown]");

         }
     }


     /* clean memory for previos memory allocation*/
     free( inquirys );
     /* close socket */
     close( socket );
}
