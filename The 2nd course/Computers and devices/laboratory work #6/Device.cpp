#include <iostream>
#include <libusb.h>
#include <iomanip>
using namespace std;

class Device {
private:
    libusb_device* dev_;
    libusb_device_descriptor desc_;
    libusb_config_descriptor* config_;
public:
    Device(libusb_device* dev);

    static string getDeviceClass(int deviceClass);

    string getInterfacesInfo() const;

    friend ostream& operator<<(ostream& s, const Device& dev);

    string getSerialNumber() const;

    ~Device();
};

Device::Device(libusb_device* dev){
    dev_ = dev;
    int errorCode = libusb_get_device_descriptor(dev_, &desc_);
    if(errorCode < 0)
        throw domain_error("Error: can't get a device descriptor, code: "
            + to_string(errorCode));
    libusb_get_config_descriptor(dev_, 0, &config_);
}

string Device::getDeviceClass(int deviceClass){
    switch(deviceClass){
    case LIBUSB_CLASS_PER_INTERFACE :
        return string("Each interface specifies its own class information");
    case LIBUSB_CLASS_AUDIO :
        return string("Audio class");
    case LIBUSB_CLASS_COMM :
        return string("Communications class");
    case LIBUSB_CLASS_HID :
        return string("Human Interface Device class");
    case LIBUSB_CLASS_PHYSICAL :
        return string("Physical");
    case LIBUSB_CLASS_PTP :
        return string("Image class");
    case LIBUSB_CLASS_PRINTER :
        return string("Printer class");
    case LIBUSB_CLASS_MASS_STORAGE :
        return string("Mass storage class");
    case LIBUSB_CLASS_HUB :
        return string("Hub class");
    case LIBUSB_CLASS_DATA :
        return string("Data class");
    case LIBUSB_CLASS_SMART_CARD :
        return string("Smart Card");
    case LIBUSB_CLASS_CONTENT_SECURITY :
        return string("Content Security");
    case LIBUSB_CLASS_VIDEO :
        return string("Video");
    case LIBUSB_CLASS_PERSONAL_HEALTHCARE :
        return string("Personal Healthcare");
    case LIBUSB_CLASS_DIAGNOSTIC_DEVICE :
        return string("Diagnostic Device");
    case LIBUSB_CLASS_WIRELESS  :
        return string("Wireless class");
    case LIBUSB_CLASS_APPLICATION :
        return string("Application class");
    case LIBUSB_CLASS_VENDOR_SPEC :
        return string("Class is vendor-specific");
    default :
        return string(" ");
    }
}

string Device::getInterfacesInfo() const {
    const libusb_interface* inter;
    const libusb_interface_descriptor* interdesc;
    stringstream interfacesInfo;
    interfacesInfo << right << setw(30) << "Number of interfaces: "
            << config_->bNumInterfaces << endl;
    for(int i = 0; i < config_->bNumInterfaces; i++){
        inter = &config_->interface[i];
        interfacesInfo << right << setw(15) << i + 1 << ") "
            << "Number of alternate settings:  "
            << inter->num_altsetting << endl;
        for(int j = 0; j < inter->num_altsetting; j++){
            interdesc = &inter->altsetting[j];
            interfacesInfo << right << setw(20) << j + 1 << ") "
                << "Interface class: "
                << getDeviceClass(interdesc->bInterfaceClass) << endl;
        }
    }
    return interfacesInfo.str();
}

string Device::getSerialNumber() const {
    libusb_device_handle* handle;
    int errorCode = libusb_open(dev_, &handle);
    if(errorCode < 0)
        throw domain_error("Error: can't open device, code: " +
            to_string(errorCode));
    unsigned char serialNumber[255];
    libusb_get_string_descriptor_ascii(handle, desc_.iSerialNumber,
        serialNumber, 255);
    libusb_close(handle);
    return string(reinterpret_cast<char*>(serialNumber));
}

ostream& operator << (ostream& s, const Device& dev){
    s << left << setw(15) << setfill(' ') << hex
        << "Device class: " << Device::getDeviceClass(dev.desc_.bDeviceClass)
            << endl << left << setfill(' ') << setw(15)
        << "Serial number: " << dev.getSerialNumber() << endl << left
            << setfill(' ') << setw(15)
        << "Vendor ID: " << right << setfill('0') << setw(4)
            << dev.desc_.idVendor << endl << left << setfill(' ') << setw(15)
        << "Product ID: " << right << setw(4) << setfill('0')
            << dev.desc_.idProduct << endl << right << setfill(' ') << setw(28)
        << "Interfaces information: " << endl << dev.getInterfacesInfo() << endl;
    return s;
}

Device::~Device(){
        libusb_free_config_descriptor(config_);
}

int main(){
    libusb_device** devs;
    libusb_context* ctx = nullptr; //session context
    int errorCode;
    ssize_t cnt;        //amount of devices
    errorCode = libusb_init(&ctx);
    if(errorCode < 0)
        throw domain_error("Error: can't initialize session, code: " +
            to_string(errorCode));
    libusb_set_debug(ctx, 3);
    cnt = libusb_get_device_list(ctx, &devs);
    if(cnt < 0)
        throw domain_error("Error: can't get device list, code: " +
            to_string(errorCode));
    for(ssize_t i = 0; i < cnt; i++){
        cout << i + 1 << ") " << endl << Device(devs[i]) << endl;
    }
    libusb_free_device_list(devs, 1);
    libusb_exit(ctx);
    return 0;
}
