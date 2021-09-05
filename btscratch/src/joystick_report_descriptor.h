#ifndef BTSCRATCH_JOYSTICK_REPORT_DESCRIPTOR_H
#define BTSCRATCH_JOYSTICK_REPORT_DESCRIPTOR_H

/*
static uint8_t joystick_report_desc[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                    // USAGE (Joystick)
    0xa1, 0x01,                    // COLLECTION (Application)

        // 88 Buttons (88bits)
        0x05, 0x09,                    //   USAGE_PAGE (Button)
            0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
            0x29, 0x58,                    //   USAGE_MAXIMUM (Button 88)
            0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
            0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
            0x75, 0x01,                    //   REPORT_SIZE (1)
            0x95, 0x58,                    //   REPORT_COUNT (88)
            0x81, 0x02,                    //   INPUT (Data,Var,Abs)
              //   no padding ( 88 )

        // left stick axis (16bits)
        0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
            0x09, 0x30,                    //   USAGE (X)
            0x09, 0x31,                    //   USAGE (Y)
            0x09, 0x33,                    //   USAGE (Clutch)
            0x09, 0x34,                    //   USAGE (Clutch)
            0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
            0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
            0x75, 0x08,                    //   REPORT_SIZE (8)
            0x95, 0x04,                    //   REPORT_COUNT (4)
            0x81, 0x02,                    //   INPUT (Data,Var,Abs)
              //   no padding ( 16 )


        // Hat switch (4bits)
        0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
            0x09, 0x39,                    //   USAGE (Hat)
            0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
            0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
            0x35, 0x00,                    //   Physical Minimum (0)
            0x46, 0x3B, 0x01,              //   Physical Maximum (315)
            0x65, 0x14,                    //   Unit (20)
            0x75, 0x04,                    //   REPORT_SIZE (4)
            0x95, 0x01,                    //   REPORT_COUNT (1)
            0x81, 0x42,                    //   INPUT (Data,Var,Abs)
              //   padding ( total 124 -> (-256) 132 (4x33) )
            0x95, 0x21,                    //   REPORT_COUNT (33)
            0x75, 0x04,                    //   REPORT_SIZE (4)
            0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)

        // Total size : 256bits -> 32bytes (JOYSTICK_SIZE)

    // 4 LEDs

    0x05, 0x0a,                    //   USAGE_PAGE (Ordinals)
    0x09, 0x01,                    //   USAGE (Instance 1)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x05, 0x08,                    //     USAGE_PAGE (LEDs)
    0x09, 0x4b,                    //     USAGE (Generic Indicator)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x02,                    //   USAGE (Instance 2)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x05, 0x08,                    //     USAGE_PAGE (LEDs)
    0x09, 0x4b,                    //     USAGE (Generic Indicator)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x03,                    //   USAGE (Instance 3)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x05, 0x08,                    //     USAGE_PAGE (LEDs)
    0x09, 0x4b,                    //     USAGE (Generic Indicator)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x09, 0x04,                    //   USAGE (Instance 4)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x05, 0x08,                    //     USAGE_PAGE (LEDs)
    0x09, 0x4b,                    //     USAGE (Generic Indicator)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
      //   padding ( 4 + 4 )
    0x95, 0x0d,                    //   REPORT_COUNT (13)
    0x75, 0x04,                    //   REPORT_SIZE (4)
    0x91, 0x01,                    // OUTPUT (Cnst,Ary,Abs)
      //  no padding ( 64 )
    0xc0                           // END_COLLECTION

    /*
    0x05, 0x0a,                    //   USAGE_PAGE (Ordinals)
    0x09, 0x01,                    //   USAGE (Instance 1)
    0xa1, 0x01,                    //   COLLECTION (Application)
    //0x85, 0x01,                      // REPORT_ID (1)
    0x05, 0x08,                    //     USAGE_PAGE (LEDs)
    0x09, 0x4b,                    //     USAGE (Generic Indicator)
    0x75, 0x01,                    //     REPORT_SIZE (8)
    0x95, 0x07,                    //     REPORT_COUNT (7)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0x91, 0x02,                    // OUTPUT (Cnst,Ary,Abs)
      //  no padding ( 64 )
    0xc0                           // END_COLLECTION

};
*/


//Changes made to source descriptor:
//Report id (1) has to specified for the BLE stack to work properly

#define JOYSTICK_REPORT_ID 0x01

static uint8_t joystick_report_desc[] = {

        0x05, JOYSTICK_REPORT_ID,                    // USAGE_PAGE (Generic Desktop)
        0x09, 0x04,                    // USAGE (Joystick)
        0xa1, 0x01,                    // COLLECTION (Application)
        0x85, 0x01,                    // REPORT_ID (1)

        // 88 Buttons (88bits)
        0x05, 0x09,                    //   USAGE_PAGE (Button)
        0x19, 0x01,                    //   USAGE_MINIMUM (Button 1)
        0x29, 0x58,                    //   USAGE_MAXIMUM (Button 88)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
        0x75, 0x01,                    //   REPORT_SIZE (1)
        0x95, 0x58,                    //   REPORT_COUNT (88)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)
        //   no padding ( 88 )
//
        // left stick axis and clutches (32 bits)
        0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //   USAGE (X)
        0x09, 0x31,                    //   USAGE (Y)
        0x09, 0x33,                    //   USAGE (Clutch)
        0x09, 0x34,                    //   USAGE (Clutch)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
        0x75, 0x08,                    //   REPORT_SIZE (8)
        0x95, 0x04,                    //   REPORT_COUNT (4)
        0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//        //   no padding ( 32 )
//
//
        // Hat switch (4bits)
        0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
        0x09, 0x39,                    //   USAGE (Hat)
        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
        0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
        0x35, 0x00,                    //   Physical Minimum (0)
        0x46, 0x3B, 0x01,              //   Physical Maximum (315)
        0x65, 0x14,                    //   Unit (20)
        0x75, 0x08,                    //   REPORT_SIZE (8)
        0x95, 0x01,                    //   REPORT_COUNT (1)
        0x81, 0x42,                    //   INPUT (Data,Var,Abs)
        // total 124

//         padding 4
//        0x95, 0x01,                    //   REPORT_COUNT (33)
//        0x75, 0x04,                    //   REPORT_SIZE (4)
//        0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)

        // Total size : 256bits -> 32bytes (JOYSTICK_SIZE)

        // 4 LEDs
//
//        0x05, 0x0a,                    //   USAGE_PAGE (Ordinals)
//        0x09, 0x01,                    //   USAGE (Instance 1)
//        0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//        0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//        0xa1, 0x02,                    //   COLLECTION (Logical)
//        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
//        0x09, 0x4b,                    //     USAGE (Generic Indicator)
//        0x75, 0x01,                    //     REPORT_SIZE (1)
//        0x95, 0x01,                    //     REPORT_COUNT (1)
//        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
//        0xc0,                          //   END_COLLECTION
//        0x09, 0x02,                    //   USAGE (Instance 2)
//        0xa1, 0x02,                    //   COLLECTION (Logical)
//        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
//        0x09, 0x4b,                    //     USAGE (Generic Indicator)
//        0x75, 0x01,                    //     REPORT_SIZE (1)
//        0x95, 0x01,                    //     REPORT_COUNT (1)
//        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
//        0xc0,                          //   END_COLLECTION
//        0x09, 0x03,                    //   USAGE (Instance 3)
//        0xa1, 0x02,                    //   COLLECTION (Logical)
//        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
//        0x09, 0x4b,                    //     USAGE (Generic Indicator)
//        0x75, 0x01,                    //     REPORT_SIZE (1)
//        0x95, 0x01,                    //     REPORT_COUNT (1)
//        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
//        0xc0,                          //   END_COLLECTION
//        0x09, 0x04,                    //   USAGE (Instance 4)
//        0xa1, 0x02,                    //   COLLECTION (Logical)
//        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
//        0x09, 0x4b,                    //     USAGE (Generic Indicator)
//        0x75, 0x01,                    //     REPORT_SIZE (1)
//        0x95, 0x01,                    //     REPORT_COUNT (1)
//        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
//        0xc0,                          //   END_COLLECTION
//        //   padding ( 4 + 4 )
//        0x95, 0x0d,                    //   REPORT_COUNT (13)
//        0x75, 0x04,                    //   REPORT_SIZE (4)
//        0x91, 0x01,                    // OUTPUT (Cnst,Ary,Abs)
        //  no padding ( 64 )
        0xc0                           // END_COLLECTION

        /*
        0x05, 0x0a,                    //   USAGE_PAGE (Ordinals)
        0x09, 0x01,                    //   USAGE (Instance 1)
        0xa1, 0x01,                    //   COLLECTION (Application)
        //0x85, 0x01,                      // REPORT_ID (1)
        0x05, 0x08,                    //     USAGE_PAGE (LEDs)
        0x09, 0x4b,                    //     USAGE (Generic Indicator)
        0x75, 0x01,                    //     REPORT_SIZE (8)
        0x95, 0x07,                    //     REPORT_COUNT (7)
        0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
        0xc0,                          //   END_COLLECTION
        0x91, 0x02,                    // OUTPUT (Cnst,Ary,Abs)
          //  no padding ( 64 )
        0xc0                           // END_COLLECTION
        */
};

//https://github.com/NordicSemiconductor/ble-sdk-arduino/blob/master/libraries/BLE/examples/ble_HID_template/USD%20HID%20Report%20Descriptor%20-%20Joystick.txt
//uint8_t ReportDescriptor[61] = {
//        0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
//        0x09, 0x04,                    // USAGE (Joystick)
//        0xa1, 0x01,                    // COLLECTION (Application)
//        0x85, 0x01,                    //   REPORT_ID (1)
//        0xa1, 0x02,                    //   COLLECTION (Logical)
//        0x09, 0x32,                    //     USAGE (Z)
//        0x09, 0x31,                    //     USAGE (Y)
//        0x09, 0x30,                    //     USAGE (X)
//        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//        0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
//        0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
//        0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
//        0x75, 0x08,                    //     REPORT_SIZE (8)
//        0x95, 0x03,                    //     REPORT_COUNT (3)
//        0x81, 0x02,                    //     INPUT (Data,Var,Abs)
//        0xc0,                          //   END_COLLECTION
//
//        0x85, 0x02,                    // REPORT_ID (2)
//
//        0xa1, 0x02,                    //   COLLECTION (Logical)
//        0x05, 0x09,                    //     USAGE_PAGE (Button)
//        0x29, 0x02,                    //     USAGE_MAXIMUM (Button 2)
//        0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
//        0x95, 0x02,                    //     REPORT_COUNT (2)
//        0x75, 0x01,                    //     REPORT_SIZE (1)
//        0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
//        0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
//        0x81, 0x02,                    //     Input (Data, Variable, Absolute)
//        0x95, 0x01,                    //     Report Count (1)
//        0x75, 0x06,                    //     Report Size (6)
//        0x81, 0x01,                    //     Input (Constant) for padding
//        0xc0,                          //   END_COLLECTION
//        0xc0                           // END_COLLECTION
//};


#endif //BTSCRATCH_JOYSTICK_REPORT_DESCRIPTOR_H
