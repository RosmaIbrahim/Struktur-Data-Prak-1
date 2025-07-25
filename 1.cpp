#include <iostream>
#include <fstream>
#include <cstdlib>  //manajemen memori, konversi angka, dan fungsi acak (rand() dan srand()).
#include <ctime>    // Untuk time

using namespace std;

// Struktur data akun
struct Akun {
    string id;
    string nama;
    string tanggal_lahir;
    string password;
    string peran;
    string alamat;
    Akun* next;
};

// Struktur data barang
struct Barang {
    string idBarang;
    string namaBarang;
    int harga;
    int stok;
    Barang* next;
};

// Struktur data keranjang
struct Keranjang {
    string idBarang;
    string namaBarang;
    int harga;
    int jumlah;
    Keranjang* next;
};

// Struktur data pesanan
struct Pesanan {
    string namaCustomer;
    string idCustomer;
    string alamatCustomer;
    int totalHarga;
    Keranjang* listBarang;
    Pesanan* next;
};

// Fungsi Deklarasi
int main();
void registrasi(Akun*& head);
void prosesLogin(Akun* akunHead, Barang*& barangHead, Pesanan*& pesananHead, Keranjang*& keranjang);
Akun* cariAkun(Akun* akunHead, string id);
void loadAkunFromFile(Akun*& akunHead);
void loadBarangFromFile(Barang*& barangHead);
void loadPesananFromFile(Pesanan*& pesananHead);
void saveAkunToFile(Akun* akunHead);
void saveBarangToFile(Barang* barangHead);
void savePesananToFile(Pesanan* pesananHead);
void menuAdmin(Barang*& barangHead, Pesanan*& pesananHead);
void menuCustomer(Akun* akun, Barang*& barangHead, Pesanan*& pesananHead, Keranjang*& keranjang);
void daftarBarangCustomer(Barang* head, Keranjang*& keranjang);
void tambahBarang(Barang*& head);
void checkoutBarang(Keranjang*& keranjang, Akun* akun, Pesanan*& pesananHead, Barang* barangHead);
void tampilkanNota(Pesanan* pesanan);
void menuKeranjang(Keranjang*& keranjang, Akun* akun, Pesanan*& pesananHead, Barang*& barangHead);
void tambahKeKeranjang(Keranjang*& keranjang, Barang* barang);
void cariBarang(Barang* head);
void hapusSemuaBarang(Keranjang*& head);
string generateIDBarang();
void tampilPesanan(Pesanan* pesananHead);
void setujuiPesanan(Pesanan*& pesananHead);
Barang* cariBarangByID(const string& id, Barang* head);
void tambahAkun(Akun*& head, const string& id, const string& nama, const string& tanggal_lahir, const string& password, const string& peran, string alamat, bool isLoad);

// Fungsi untuk mengubah karakter ke huruf besar
char myToUpper(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 'A';
    return c;
}

// Fungsi untuk mengubah karakter menjadi digit sesuai aturan
int customCharToDigit(char c) {
    return (myToUpper(c) - 'A' + 1) % 10;
}

// Fungsi untuk mendapatkan angka dari tanggal lahir
int extractDigitFromDate(const string& tanggal_lahir) {
    int tgl = 0, bln = 0, thn = 0; // Variabel untuk menyimpan tanggal, bulan, dan tahun
    int index = 0, temp = 0; 
    
    for (int i = 0; i < tanggal_lahir.length(); i++) {
        if (tanggal_lahir[i] >= '0' && tanggal_lahir[i] <= '9') { // Jika karakter adalah angka
            temp = temp * 10 + (tanggal_lahir[i] - '0'); // Konversi karakter ke angka
        }
        else { // Jika karakter bukan angka (misalnya pemisah '-','/')
            if (index == 0)
                tgl = temp; // Simpan nilai sebagai tanggal
            else if (index == 1)
                bln = temp; // Simpan nilai sebagai bulan
            temp = 0; // Reset nilai sementara untuk tahun
            index++; // Pindah ke bagian berikutnya (bulan/tahun)
        }
    }
    thn = temp; // Simpan bagian terakhir sebagai tahun
    return (tgl + bln + thn) % 10; // Mengembalikan hasil penjumlahan digit yang dimodulo 10
}

// Fungsi untuk mengambil dua digit pertama dari password
void extractDigitsFromPassword(const string& password, int& digit4, int& digit5) {
    digit4 = -1; // Inisialisasi digit4 dengan -1 sebagai nilai default
    digit5 = -1; // Inisialisasi digit5 dengan -1 sebagai nilai default
    int count = 0; // Counter untuk melacak jumlah digit yang ditemukan
    
    for (int i = 0; i < password.length(); i++) { // Loop melalui setiap karakter dalam password
        char c = password[i]; // Ambil karakter saat ini
        if (c >= '0' && c <= '9') { // Periksa apakah karakter adalah digit
            if (count == 0)
                digit4 = c - '0'; // Simpan digit pertama yang ditemukan
            else if (count == 1) {
                digit5 = c - '0'; // Simpan digit kedua yang ditemukan
                break; // Hentikan loop setelah menemukan dua digit
            }
            count++; // Tambahkan hitungan digit yang ditemukan
        }
    }
    if (digit4 == -1)
        digit4 = 0; // Jika tidak ada digit pertama yang ditemukan, atur ke 0
    if (digit5 == -1)
        digit5 = 0; // Jika tidak ada digit kedua yang ditemukan, atur ke 0
}
// Fungsi untuk membuat ID berdasarkan nama, tanggal lahir, password, dan peran
string generateID(const string& nama, const string& tanggal_lahir, const string& password, const string& peran) {
    int digit1 = customCharToDigit(nama[0]); // Ambil digit pertama dari karakter pertama nama
    int digit2 = customCharToDigit(nama[nama.length() - 1]); // Ambil digit kedua dari karakter terakhir nama
    int digit3 = extractDigitFromDate(tanggal_lahir); // Ambil digit ketiga dari tanggal lahir
    int digit4, digit5;
    extractDigitsFromPassword(password, digit4, digit5); // Ambil digit keempat dan kelima dari password
    int digit6 = (peran == "Admin") ? 0 : 1; // Tentukan digit keenam berdasarkan peran (0 untuk Admin, 1 untuk lainnya)

    // Konversi integer ke string secara manual
    char id[7];
    id[0] = digit1 + '0';
    id[1] = digit2 + '0';
    id[2] = digit3 + '0';
    id[3] = digit4 + '0';
    id[4] = digit5 + '0';
    id[5] = digit6 + '0';
    id[6] = '\0'; // Menambahkan null terminator untuk string C-style 

    return string(id); // Mengembalikan hasil ID dalam bentuk string
}


// Fungsi untuk menghasilkan ID barang yang baru
string generateIDBarang() {
    string idBarang = "";  // Inisialisasi string kosong untuk menyimpan ID barang
    srand(static_cast<unsigned int>(time(0))); 
    
    for (int i = 0; i < 6; i++) { // Loop untuk membuat 6 digit angka acak
        idBarang += to_string(rand() % 10);  // Menghasilkan angka acak 0-9 dan mengonversinya ke string
    }
    return idBarang; // Mengembalikan ID barang dalam bentuk string
}

// Fungsi untuk menambahkan barang ke dalam daftar barang
void tambahBarang(Barang*& head) {
    string namaBarang;
    int harga, stok;
    char lagi;

    do {
        cout << "\n------ Tambah Barang ------" << endl;

        cin.ignore();
        cout << "Masukkan Nama Barang: ";
        getline(cin, namaBarang);
        cout << "Masukkan Harga Barang: ";
        cin >> harga;
        cout << "Masukkan Stok Barang: ";
        cin >> stok;

        string idBarang = generateIDBarang();  // Generate ID acak
        Barang* newBarang = new Barang{ idBarang, namaBarang, harga, stok, nullptr };

        if (!head) {
            head = newBarang;
        }
        else {
            Barang* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newBarang;
        }

        cout << "Barang berhasil ditambahkan!" << endl;
        cout << "ID Barang: " << idBarang << endl;

        cout << "Apakah Anda ingin menambahkan barang lain? (y/n): ";
        cin >> lagi;
    } while (lagi == 'y' || lagi == 'Y');
}

// Fungsi untuk menampilkan semua pesanan
void tampilPesanan(Pesanan* pesananHead) {
    if (pesananHead == nullptr) {
        cout << "Belum ada pesanan." << endl;
        return;
    }

    while (pesananHead != nullptr) {
        cout << "Pesanan dari: " << pesananHead->namaCustomer << endl;
        cout << "ID Customer: " << pesananHead->idCustomer << endl;
        cout << "Alamat: " << pesananHead->alamatCustomer << endl;
        cout << "Total Harga: " << pesananHead->totalHarga << endl;
        cout << "Barang yang dipesan: " << endl;

        Keranjang* barangTemp = pesananHead->listBarang;
        while (barangTemp) {
            cout << " - " << barangTemp->namaBarang << " x" << barangTemp->jumlah << " | Rp " << barangTemp->harga * barangTemp->jumlah << endl;
            barangTemp = barangTemp->next;
        }

        cout << "--------------------------" << endl;
        pesananHead = pesananHead->next;
    }
}

// Fungsi untuk memproses pesanan
void setujuiPesanan(Pesanan*& pesananHead) {
    if (pesananHead == nullptr) {
        cout << "Tidak ada pesanan untuk diproses!" << endl;
        return;
    }

    Pesanan* temp = pesananHead;
    cout << "----- Pesanan Pertama -----" << endl;
    cout << "Nama: " << temp->namaCustomer << endl;
    cout << "ID: " << temp->idCustomer << endl;
    cout << "Alamat: " << temp->alamatCustomer << endl;
    cout << "=============================" << endl;

    Keranjang* barangTemp = temp->listBarang;
    while (barangTemp) {
        cout << "ID: " << barangTemp->idBarang << ", Nama: " << barangTemp->namaBarang << ", Harga: " << barangTemp->harga << ", Quantity: " << barangTemp->jumlah << endl;
        barangTemp = barangTemp->next;
    }
    cout << "Total: " << temp->totalHarga << endl;

    cout << "1. Setujui Pesanan" << endl;
    cout << "2. Kembali" << endl;
    cout << "Pilihan Anda: ";
    int pilihan;
    cin >> pilihan;

    if (pilihan == 1) {
        Pesanan* approvedOrder = pesananHead;
        pesananHead = pesananHead->next;  // Keluar dari daftar pesanan
        delete approvedOrder;             // Menghapus pesanan yang disetujui
        cout << "Pesanan telah disetujui dan sedang dalam proses pengiriman!" << endl;
    }
    else {
        cout << "Kembali ke menu admin." << endl;
    }
}

// Fungsi untuk menampilkan daftar barang untuk admin
void daftarBarangAdmin(Barang* head) {
    if (head == nullptr) {
        cout << "Belum ada barang yang tersedia!" << endl;
        return;
    }

    system("cls"); //untuk membersihkan layar
    cout << "------ Daftar Barang ------" << endl;

    Barang* temp = head;
    while (temp != nullptr) {
        cout << "ID: " << temp->idBarang << endl;
        cout << "Nama: " << temp->namaBarang << endl;
        cout << "Harga: " << temp->harga << endl;
        cout << "Stok: " << temp->stok << endl;
        cout << "---------------------------" << endl;
        temp = temp->next;
    }
}

// Fungsi menu admin
void menuAdmin(Barang*& barangHead, Pesanan*& pesananHead) {
    int pilihan;
    do {
        cout << "====== MENU ADMIN ======" << endl;
        cout << "1. Tambah Barang" << endl;
        cout << "2. Daftar Barang" << endl;
        cout << "3. Pesanan" << endl;
        cout << "4. Logout" << endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
        case 1:
            tambahBarang(barangHead);
            break;
        case 2:
            daftarBarangAdmin(barangHead);
            break;
        case 3:
            tampilPesanan(pesananHead);
            // Menambahkan opsi untuk menyetujui pesanan
            setujuiPesanan(pesananHead);
            break;
        case 4:
            cout << "Logout berhasil!" << endl;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 4);
}

// Fungsi untuk mencari barang berdasarkan ID/nama
void cariBarang(Barang* head) {
    if (head == nullptr) {
        cout << "Belum ada barang yang tersedia!" << endl;
        return;
    }

    string cari;
    cout << "\n------ Cari Barang ------" << endl;
    cout << "Masukkan ID/Nama Barang: ";
    cin.ignore();
    getline(cin, cari);

    Barang* temp = head;
    bool ditemukan = false;
    while (temp != nullptr) {
        if (temp->idBarang == cari || temp->namaBarang == cari) {
            cout << "\n=============================" << endl;
            cout << "ID: " << temp->idBarang << endl;
            cout << "Nama: " << temp->namaBarang << endl;
            cout << "Harga: " << temp->harga << endl;
            cout << "Stok: " << temp->stok << endl;
            cout << "=============================" << endl;
            ditemukan = true;
            break;  // Keluar dari loop setelah menemukan barang
        }
        temp = temp->next;
    }

    if (!ditemukan) {
        cout << "Barang dengan ID/Nama \"" << cari << "\" tidak ditemukan!" << endl;
    }
}

// Fungsi untuk menampilkan daftar barang bagi customer
void daftarBarangCustomer(Barang* head, Keranjang*& keranjang) {
    if (head == nullptr) {
        cout << "Belum ada barang yang tersedia!" << endl;
        return;
    }

    Barang* temp = head;
    Barang* prev = nullptr;  // Tambahkan pointer ke barang sebelumnya
    int pilihan;

    while (temp != nullptr) {
        cout << "\nID: " << temp->idBarang << endl;
        cout << "Nama: " << temp->namaBarang << endl;
        cout << "Harga: " << temp->harga << endl;
        cout << "Stok: " << temp->stok << endl;
        cout << "---------------------------" << endl;

        cout << "1. Tambah ke Keranjang" << endl;
        cout << "2. Next" << endl;
        cout << "3. Prev" << endl;  // Tambahkan pilihan Prev
        cout << "4. Keluar" << endl;
        cout << "Pilihan: ";
        cin >> pilihan;

        switch (pilihan) {
        case 1:
            tambahKeKeranjang(keranjang, temp);
            break;
        case 2:
            prev = temp;        // Simpan barang saat ini sebagai 'prev'
            temp = temp->next;  // Lanjut ke barang berikutnya
            if (temp == nullptr) {  // Jika sudah di akhir list
                cout << "Sudah mencapai akhir daftar barang." << endl;
                temp = prev;        // Kembali ke barang sebelumnya
                prev = (temp == head) ? nullptr : head;  // Update prev jika perlu
            }
            break;
        case 3:  // Pilihan Prev
            if (prev == nullptr) {  // Jika sudah di awal list
                cout << "Sudah berada di awal daftar barang." << endl;
            }
            else {
                temp = prev;  // Kembali ke barang sebelumnya
                Barang* current = head;
                prev = nullptr;

                while (current != nullptr && current->next != temp) {
                    prev = current;
                    current = current->next;
                }
            }
            break;
        case 4:
            return;  // Keluar dari daftar
        default:
            cout << "Pilihan tidak valid! Coba lagi." << endl;
        }
    }
}

// Fungsi untuk mencari barang berdasarkan ID
Barang* cariBarangByID(const string& id, Barang* head) {
    while (head != nullptr) {
        if (head->idBarang == id) {
            return head;
        }
        head = head->next;
    }
    return nullptr;
}

// Fungsi untuk menambahkan barang ke keranjang
void tambahKeKeranjang(Keranjang*& keranjang, Barang* barang) {
    if (barang->stok <= 0) {
        cout << "Stok tidak mencukupi!" << endl;
        return;
    }

    int jumlah;
    cout << "Masukkan jumlah barang yang ingin dibeli: ";
    cin >> jumlah;

    if (jumlah > barang->stok) {
        cout << "Stok tidak mencukupi! Hanya ada " << barang->stok << " item tersedia." << endl;
        return;
    }

    // Kurangi stok barang
    barang->stok -= jumlah;

    Keranjang* newBarang = new Keranjang;
    newBarang->idBarang = barang->idBarang;
    newBarang->namaBarang = barang->namaBarang;
    newBarang->harga = barang->harga;
    newBarang->jumlah = jumlah;
    newBarang->next = keranjang;
    keranjang = newBarang;

    cout << "Barang berhasil ditambahkan ke keranjang!" << endl;
}

// Pastikan ada satu definisi untuk fungsi ini, dan hapus yang lain
void hapusSemuaBarang(Keranjang*& head) {
    while (head != nullptr) {
        Keranjang* temp = head;
        head = head->next;
        delete temp;
    }
}

void checkoutBarang(Keranjang*& keranjang, Akun* akun, Pesanan*& pesananHead, Barang* barangHead) {
    if (keranjang == nullptr) {
        cout << "\nKeranjang kosong! Tidak ada barang yang bisa di-checkout.\n";
        return;
    }

    string alamatPengguna;
    cout << "\nMasukkan Alamat Anda: ";
    cin.ignore();
    getline(cin, alamatPengguna);

    if (alamatPengguna.empty()) {
        cout << "Alamat tidak boleh kosong!\n";
        return;
    }

    Pesanan* pesananBaru = new Pesanan; // Alokasi memori untuk pesanan baru
pesananBaru->namaCustomer = akun->nama; // Mengisi nama pelanggan berdasarkan akun yang login
pesananBaru->idCustomer = akun->id; // Mengisi ID pelanggan berdasarkan akun yang login
pesananBaru->alamatCustomer = alamatPengguna; // Mengisi alamat pelanggan dari input pengguna
pesananBaru->listBarang = nullptr; // Inisialisasi daftar barang dalam pesanan sebagai kosong
pesananBaru->next = nullptr; // Inisialisasi pointer next sebagai null (pesanan baru belum terhubung ke daftar)
pesananBaru->totalHarga = 0; // Set total harga pesanan ke 0 karena belum ada barang yang ditambahkan

    Keranjang* tempKeranjang = keranjang;
    Keranjang* tailPesanan = nullptr;
    while (tempKeranjang != nullptr) {
        // Cari barang di daftar barang menggunakan ID
        Barang* barangAsli = cariBarangByID(tempKeranjang->idBarang, barangHead);
        if (barangAsli == nullptr) {
            cout << "Barang dengan ID " << tempKeranjang->idBarang << " tidak ditemukan dalam daftar barang.\n";
            // Handle error: Batalkan checkout atau skip barang ini
            tempKeranjang = tempKeranjang->next;
            continue;
        }

        // Buat node baru untuk pesanan
        Keranjang* barangPesanan = new Keranjang;
        barangPesanan->idBarang = tempKeranjang->idBarang;
        barangPesanan->namaBarang = tempKeranjang->namaBarang;
        barangPesanan->harga = tempKeranjang->harga;
        barangPesanan->jumlah = tempKeranjang->jumlah;
        barangPesanan->next = nullptr;

        // Tambahkan barang ke daftar pesanan
        if (pesananBaru->listBarang == nullptr) {
            pesananBaru->listBarang = barangPesanan;
            tailPesanan = barangPesanan;
        }
        else {
            tailPesanan->next = barangPesanan;
            tailPesanan = barangPesanan;
        }

        pesananBaru->totalHarga += barangPesanan->harga * barangPesanan->jumlah;
        tempKeranjang = tempKeranjang->next;
    }

    // Tambahkan pesanan ke daftar pesanan
    if (pesananHead == nullptr) {
        pesananHead = pesananBaru;
    }
    else {
        Pesanan* temp = pesananHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = pesananBaru;
    }

    // Kosongkan keranjang setelah checkout
    hapusSemuaBarang(keranjang);

    cout << "\nCheckout berhasil! Total harga: Rp " << pesananBaru->totalHarga << endl;
    cout << "Pesanan Anda sedang diproses.\n";
}
void tampilkanNota(Pesanan *pesanan)
{
    if (pesanan == nullptr)
    {
        cout << "Tidak ada pesanan untuk ditampilkan." << endl;
        return;
    }

    cout << "\n======== NOTA PEMBELIAN ========\n";
    cout << "Nama Pelanggan: " << pesanan->namaCustomer << endl;
    cout << "ID Pelanggan: " << pesanan->idCustomer << endl;
    cout << "Alamat Pengiriman: " << pesanan->alamatCustomer << endl;
    cout << "----------------------------------\n";
    cout << "Daftar Barang:\n";

    Keranjang *barang = pesanan->listBarang;
    int totalBayar = 0;
    while (barang != nullptr)
    {
        int subtotal = barang->harga * barang->jumlah;
        cout << barang->namaBarang << " x " << barang->jumlah << " @ Rp " << barang->harga << " = Rp " << subtotal << endl;
        totalBayar += subtotal;
        barang = barang->next;
    }

    cout << "----------------------------------\n";
    cout << "Total Bayar: Rp " << totalBayar << endl;
    cout << "======== TERIMA KASIH! ========\n";
}
void menuKeranjang(Keranjang *&keranjang, Akun *akun, Pesanan *&pesananHead, Barang *&barangHead)
{
    int pilihan;
    do
    {
        cout << "\n======== MENU KERANJANG ========\n";
        if (keranjang == nullptr)
        {
            cout << "Keranjang Anda kosong.\n";
        }
        else
        {
            Keranjang *temp = keranjang;
            int i = 1;
            while (temp != nullptr)
            {
                cout << i << ". " << temp->namaBarang << " - Jumlah: " << temp->jumlah << " - Harga: Rp " << temp->harga * temp->jumlah << endl;
                temp = temp->next;
                i++;
            }
        }
        cout << "----------------------------------\n";
        cout << "1. Checkout" << endl;
        cout << "2. Hapus Semua Barang di Keranjang" << endl;
        cout << "3. Kembali ke Menu Customer" << endl;
        cout << "Pilih opsi: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            checkoutBarang(keranjang, akun, pesananHead, barangHead);
            break;
        case 2:
            hapusSemuaBarang(keranjang);
            cout << "Semua barang di keranjang telah dihapus.\n";
            break;
        case 3:
            cout << "Kembali ke Menu Customer.\n";
            break;
        default:
            cout << "Opsi tidak valid. Silakan coba lagi.\n";
        }
    } while (pilihan != 3);
}
void tambahAkun(Akun *&head, const string &id, const string &nama,
                const string &tanggal_lahir, const string &password,
                const string &peran, string alamat, bool isLoad = false)
{
    Akun *newAkun = new Akun;
    newAkun->id = id;
    newAkun->nama = nama;
    newAkun->tanggal_lahir = tanggal_lahir;
    newAkun->password = password;
    newAkun->peran = peran;
    newAkun->alamat = alamat;
    newAkun->next = nullptr;

    if (head == nullptr)
    {
        head = newAkun;
    }
    else
    {
        Akun *temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newAkun;
    }

    if (!isLoad)
    {
        cout << "Akun berhasil ditambahkan!" << endl;
    }
}
Akun *cariAkun(Akun *akunHead, string id)
{
    Akun *current = akunHead;
    while (current != nullptr)
    {
        if (current->id == id)
        {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}
void registrasi(Akun *&head)
{
    string nama, tanggal_lahir, password, peran, alamat;
    cout << "Masukkan Nama: ";
    cin.ignore();
    getline(cin, nama);
    cout << "Masukkan Tanggal Lahir (dd/mm/yyyy): ";
    getline(cin, tanggal_lahir);
    cout << "Masukkan Password: ";
    getline(cin, password);
    cout << "Masukkan Peran (Admin/Customer): ";
    getline(cin, peran);
    cout << "Masukkan Alamat: ";
    getline(cin, alamat);

    string id = generateID(nama, tanggal_lahir, password, peran);
    tambahAkun(head, id, nama, tanggal_lahir, password, peran, alamat);
    cout << "ID Anda: " << id << endl;  // Menampilkan ID setelah registrasi
}
void menuCustomer(Akun *akun, Barang *&barangHead, Pesanan *&pesananHead, Keranjang *&keranjang)
{
    int pilihan;
    do
    {
        cout << "\n====== MENU CUSTOMER ======" << endl;
        cout << "Selamat datang, " << akun->nama << "!" << endl;
        cout << "1. Daftar Barang" << endl;
        cout << "2. Cari Barang" << endl;
        cout << "3. Keranjang" << endl;
        cout << "4. Logout" << endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan)
        {
        case 1:
            daftarBarangCustomer(barangHead, keranjang);
            break;
        case 2:
            cariBarang(barangHead);
            break;
        case 3:
            menuKeranjang(keranjang, akun, pesananHead, barangHead);
            break;
        case 4:
            cout << "Logout berhasil!" << endl;
            break;
        default:
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 4);
}
void prosesLogin(Akun *akunHead, Barang *&barangHead, Pesanan *&pesananHead,
                 Keranjang *&keranjang)
{
    string id, password;
    cout << "Masukkan ID: ";
    cin >> id;
    cout << "Masukkan Password: ";
    cin >> password;

    Akun *akun = cariAkun(akunHead, id);
    if (akun != nullptr && akun->password == password)
    {
        cout << "Login berhasil!" << endl;
        if (akun->peran == "Admin")
        {
            menuAdmin(barangHead, pesananHead);
        }
        else if (akun->peran == "Customer")
        {
            menuCustomer(akun, barangHead, pesananHead, keranjang);
        }
    }
    else
    {
        cout << "ID atau Password salah!" << endl;
    }
}
void loadAkunFromFile(Akun *&akunHead)
{
    ifstream file("akun.txt");
    if (file.is_open())
    {
        string id, nama, tanggal_lahir, password, peran, alamat;
        while (getline(file, id))
        {
            getline(file, nama);
            getline(file, tanggal_lahir);
            getline(file, password);
            getline(file, peran);
            getline(file, alamat);
            tambahAkun(akunHead, id, nama, tanggal_lahir, password, peran, alamat, true);
        }
        file.close();
    }
    else
    {
        //cout << "Tidak dapat membuka file akun.txt" << endl;
    }
}
void loadBarangFromFile(Barang *&barangHead)
{
    ifstream file("barang.txt");
    if (file.is_open())
    {
        string idBarang, namaBarang;
        int harga, stok;
        while (getline(file, idBarang))
        {
            getline(file, namaBarang);
            file >> harga >> stok;
            file.ignore(); // Membersihkan newline setelah membaca angka
            Barang *newBarang = new Barang;
            newBarang->idBarang = idBarang;
            newBarang->namaBarang = namaBarang;
            newBarang->harga = harga;
            newBarang->stok = stok;
            newBarang->next = nullptr;

            if (barangHead == nullptr)
            {
                barangHead = newBarang;
            }
            else
            {
                Barang *temp = barangHead;
                while (temp->next != nullptr)
                {
                    temp = temp->next;
                }
                temp->next = newBarang;
            }
        }
        file.close();
    }
    else
    {
        //cout << "Tidak dapat membuka file barang.txt" << endl;
    }
}
void loadPesananFromFile(Pesanan *&pesananHead)
{
    ifstream file("pesanan.txt");
    if (file.is_open())
    {
        string namaCustomer, idCustomer, alamatCustomer;
        int totalHarga;
        while (getline(file, namaCustomer))
        {
            getline(file, idCustomer);
            getline(file, alamatCustomer);
            file >> totalHarga;
            file.ignore(); // Membersihkan newline setelah membaca angka

            Pesanan *newPesanan = new Pesanan;
            newPesanan->namaCustomer = namaCustomer;
            newPesanan->idCustomer = idCustomer;
            newPesanan->alamatCustomer = alamatCustomer;
            newPesanan->totalHarga = totalHarga;
            newPesanan->listBarang = nullptr;
            newPesanan->next = nullptr;

            if (pesananHead == nullptr)
            {
                pesananHead = newPesanan;
            }
            else
            {
                Pesanan *temp = pesananHead;
                while (temp->next != nullptr)
                {
                    temp = temp->next;
                }
                temp->next = newPesanan;
            }
        }
        file.close();
    }
    else
    {
        //cout << "Tidak dapat membuka file pesanan.txt" << endl;
    }
}
void saveAkunToFile(Akun *akunHead)
{
    ofstream file("akun.txt");
    if (file.is_open())
    {
        Akun *temp = akunHead;
        while (temp != nullptr)
        {
            file << temp->id << endl;
            file << temp->nama << endl;
            file << temp->tanggal_lahir << endl;
            file << temp->password << endl;
            file << temp->peran << endl;
            file << temp->alamat << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Data akun berhasil disimpan ke file." << endl;
    }
    else
    {
        cout << "Tidak dapat membuka file untuk menyimpan data akun." << endl;
    }
}
void saveBarangToFile(Barang *barangHead)
{
    ofstream file("barang.txt");
    if (file.is_open())
    {
        Barang *temp = barangHead;
        while (temp != nullptr)
        {
            file << temp->idBarang << endl;
            file << temp->namaBarang << endl;
            file << temp->harga << " " << temp->stok << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Data barang berhasil disimpan ke file." << endl;
    }
    else
    {
        cout << "Tidak dapat membuka file untuk menyimpan data barang." << endl;
    }
}
void savePesananToFile(Pesanan *pesananHead)
{
    ofstream file("pesanan.txt");
    if (file.is_open())
    {
        Pesanan *temp = pesananHead;
        while (temp != nullptr)
        {
            file << temp->namaCustomer << endl;
            file << temp->idCustomer << endl;
            file << temp->alamatCustomer << endl;
            file << temp->totalHarga << endl;
            temp = temp->next;
        }
        file.close();
        cout << "Data pesanan berhasil disimpan ke file." << endl;
    }
    else
    {
        cout << "Tidak dapat membuka file untuk menyimpan data pesanan." << endl;
    }
}
int main() {
    Akun *akunHead = nullptr;
    Barang *barangHead = nullptr;
    Pesanan *pesananHead = nullptr;
    Keranjang *keranjang = nullptr;

    loadAkunFromFile(akunHead);
    loadBarangFromFile(barangHead);
    loadPesananFromFile(pesananHead);

    int pilihan;
    do {
        cout << "====== SELAMAT DATANG ======" << endl;
        cout << "1. Registrasi" << endl;
        cout << "2. Login" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihan Anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                registrasi(akunHead);
                
                break;
            case 2:
                prosesLogin(akunHead, barangHead, pesananHead, keranjang);
                
                break;
            case 3:
                cout << "Terima kasih telah menggunakan aplikasi ini!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Coba lagi." << endl;
        }
    } while (pilihan != 3);

    return 0;
}
