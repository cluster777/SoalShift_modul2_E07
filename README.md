# Soal Shift Modul 2
## Nomor 1
Nomor 1 diminta untuk mengganti nama sebuah file {namafile}.png menjadi {namafile}_grey.png.
Untuk melakukannya, maka dibuka folder di mana file-file png tersebut berada.

```C
DIR *d;
d = opendir("/home/arino/Pictures/Modul2");
```

Kemudian ambil nama file yang ada didalamnya, concatenate dengan "_grey", kemudian ganti nama filenya.
Implementasinya seperti berikut

```C
struct dirent *dir;
while ((dir = readdir(d)) != NULL)
{
    char *fname = dir->d_name;
    char *tmp = strdup(fname);
    char *dot = strrchr(tmp, '.');
    if (!dot)
        continue;   //Tidak punya ekstensi
    char *ext = strdup(dot + 1);
    if (strcmp(ext, "png") == 0)
    {
        //Ekstensinya png
        *dot = '\0';
        char newdir[256], origdir[256];
        strcpy(newdir, "/home/arino/modul2/gambar/");
        strcpy(origdir, "/home/arino/Pictures/Modul2/");
        strcat(origdir, fname);
        strcat(tmp, "_grey.");
        strcat(newdir, tmp);
        strcat(newdir, ext);
        rename(origdir, newdir);
    }
}
```

Untuk path-nya silakan sesuaikan dengan komputer masing-masing.

## Nomor 2
Nomor 2 diminta untuk mendeteksi sebuah owner dan group dari sebuah file.
File tersebut akan dihapus jika owner dan groupnya "www-data".
Hal tersebut dimonitor selama 3 detik sekali.
Untuk mendapatkan owner dan group dari sebuah file, dilakukan `stat()`, `getpwuid()`, dan `getgrgid()`.
Implementasinya sebagai berikut.

```C
struct stat st;
stat("/home/arino/hatiku/elen.ku", &st);
struct passwd *pwd = getpwuid(st.st_uid);   //Owner
struct group *gr = getgrgid(st.st_gid);     //Group
```

Kemudian jika owner dan groupnya adalah "www-data", maka hapus file tersebut.

```C
if ((strcmp(gr->gr_name, "www-data") == 0) && (strcmp(pwd->pw_name, "www-data") == 0))
{
    remove("/home/arino/hatiku/elen.ku");
}
```

Sesuaikan pathnya sesuai komputer masing-masing.

## Nomor 3
Nomor 3 diminta untuk mengekstrak sebuah zip, 
membuat `daftar.txt` di mana isinya adalah file yang berekstensi txt di dalam folder yang diekstrak.
Untuk melakukan hal-hal diatas, diperlukan 3 process, yang mengekstrak, yang mendata, dan menuliskannya ke file.
Karena tidak ada shell, maka digunakan `ls` dan `grep` untuk mencarinya.

### unzip
```C
char *argv[] = {"unzip", "campur2.zip", NULL};
execv("/usr/bin/unzip", argv);
```

### ls
```C
close(STDOUT_FILENO);
dup2(fd1[1], 1);
close(fd1[0]);
close(fd1[1]);
close(fd2[0]);
close(fd2[1]);
chdir("./campur2");
char *argv[] = {"ls", NULL};
execv("/bin/ls", argv);
```

### grep
```C
close(STDOUT_FILENO);
close(STDIN_FILENO);
dup2(fd1[0], 0);
dup2(fd2[1], 1);
close(fd1[0]);
close(fd1[1]);
close(fd2[0]);
close(fd2[1]);
char *argv[] = {"grep", "txt$", NULL};
execv("/bin/grep", argv);
```

### cat
```C
close(STDIN_FILENO);
close(STDOUT_FILENO);
dup2(fd2[0], 0);
dup2(fileout, 1);
close(fd1[0]);
close(fd1[1]);
close(fd2[0]);
close(fd2[1]);
char *argv[] = {"cat", NULL};
execv("/bin/cat", argv);
```

Untuk menyalurkan data diantara proses-proses tersebut, digunakan pipe `fd1` dan `fd2`.
`fd1` menghubungkan `ls` dengan `grep`, `fd2` menghubungkan `grep` dengan `cat`.

## Nomor 4
Nomor 4 diminta untuk memonitor sebuah file.
Jika file tersebut pernah dibuka selama 30 detik yang lalu, maka buat file baru.
Program ini berjalan selama 5 detik sekali.

Pertama cek dulu waktu akses terakhir.
```C
struct stat attr;
stat("/root/Documents/ss2/makan_enak.txt", &attr);
time_t a = attr.st_atime;
time_t b = time(0);
```
Kemudian bandingkan, jika pernah dibuka selama 30 detik yang lalu, maka mulai buat file-filenya
```C
if (b-30<=a)
{
    counter[counter.size()-1]++;
    for (int i=counter.size()-1;i>=0;i--)
    {
        if (counter[i]-'9'>0 && i>0)
        {
            counter[i]='0';
            counter[i-1]++;
        }
        else if (counter[i]-'9'>0 && i==0)
        {
            counter[i]='0';
            counter="1"+counter;
        }
    }
    ofstream outfile;
    outfile.open("/root/Documents/ss2/makansehat"+counter+".txt");
    outfile<<"diet woy";
    outfile.close();
}
```

Sesuaikan path sesuai komputer masing-masing

## Nomor 5
Nomor 5 diminta untuk mencatat log setiap menit dari `syslog`.
Log tersebut disimpan di folder `home/$USER/log/{dd:MM:yyyy-hh:mm}/log#.log`.
Folder dibuat setiap 30 menit sekali, sedangkan filenya dibuat setiap menit.

Untuk membuat folder, digunakan proses sendiri untuk membuatnya.
```C
if (child_id == 0)
{
    string now=currentDateTime();
    ofstream outfile;
    outfile.open("dsccd.txt");
    now="/root/log/"+now;
    outfile<<now;
    outfile.close();
    char *argv[4]={"mkdir","-p",(char*)(now.c_str()),NULL};
    execv("/bin/mkdir",argv );
}
```
Dan untuk filenya
```C
string namafile;
ifstream infile;
infile.open("dsccd.txt");
infile>>namafile;
infile.close();
string counter="0";
while (1)
{
    counter[counter.size()-1]++;
    for (int i=counter.size()-1;i>=0;i--)
    {
        if(counter[i]-'9'>0 && i>0)
        {
        counter[i]='0';
        counter[i-1]++;
        }
        else if(counter[i]-'9'>0 && i==0)
        {
        counter[i]='0';
        counter="1"+counter;
        }
    }
    char data;
    ifstream infile;
    ofstream outfile;
    infile.open("/var/log/syslog");
    outfile.open(namafile+"/log"+counter+".log");
    while((infile>>data))
    {
        if (infile.eof())
            break;
        outfile<<data;
    }
    infile.close();
    outfile.close();
}
```
Untuk menghentikannya tinggal
```C
char *arg[3] = {"pkill","soal5a",NULL};
execv("/usr/bin/pkill",arg);
```
