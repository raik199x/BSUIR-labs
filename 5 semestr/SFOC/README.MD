# Quartus installation guide (linux debian)

Since installation such pain in ass i decided to write guide how i done it myself:

First, you need to download [Quartus web](https://www.intel.com/content/www/us/en/software-kit/711790/intel-quartus-ii-web-edition-design-software-version-13-0sp1-for-linux.html) (since it's fully free),
recommend to download version 13.1.

Second, before installation of quartus, you need to install compatibility library for 32-bit libraries, for that, visit [this site](http://ppa.launchpad.net/linuxuprising/libpng12/ubuntu/pool/main/libp/libpng/) and find *libpng12-0_1.2.54-1ubuntu1.1+1~ppa0~eoan_amd64.deb* file, download and run

  sudo dpkg -i libpng12-0_1.2.54-1ubuntu1.1+1~ppa0~eoan_amd64.deb

Third, install some additional libraries

  sudo apt-get install libxft2 libxft2:i386 libncurses5:i386 ia32-libs libxtst6:i386

if your package manager can't find this libs, type this command

  sudo dpkg --add-architecture i386

and then repeat installation of libs

And after that you can install and run quartus, if you would not able to simulate time scheme with error of "modelsim" you need to duplicate path into modelsim from modelsim-alter, to do this go to: 
tools ->  options -> eda tool option

Thats all, after all this steps i could use quartus freely

---

Links that helped me:

https://www.linuxuprising.com/2018/05/fix-libpng12-0-missing-in-ubuntu-1804.html

https://stackoverflow.com/questions/17005654/error-while-loading-shared-libraries-libncurses-so-5

https://stackoverflow.com/questions/31908525/modelsim-altera-error

https://askubuntu.com/questions/895897/error-while-loading-shared-libraries-libpng12-so-0 (for Pos Os! better to build using sources)

---

p.s. But as practic show, it's better to install virtual mashine with quartus 9.1 because of missing feature in older versions of program

---

Marks:

1. 8+/10

2. 9-/10

3. 7-/10

4. 8+/10

5. TBA

6. TBA

7. TBA

8. FUCK IT
