#!/usr/bin/zsh

//TODO DDEV için özel old_cms.sh yazılacak.

# chmod +x old_cms.sh
#add alias into .bashrc or .zshrc -> alias oldcms='/home/tnrdll/Shell\ Scripts/my_shell_scripts/old_cms.sh'


LGR='\033[1;32m' # LIGHT GREEN
NOC='\033[0m'    # NO COLOR
PUR='\033[0;35m' # PURPLE
RED='\033[0;31m' # RED



#t: tar file path
#s: sql file path
#command: ./cms_local_setup.sh -n pia -t cms.tar.gz -z file.sql.gz
while getopts n:t:z: flag
do
    case "${flag}" in
        n) dir_name=${OPTARG};;
        t) tar_file=${OPTARG};;
        z) database_file=${OPTARG};;
        *) echo "Invalid option: -$flag" ;;
    esac
done

#echo ${tar_file}
#echo ".tar path giriniz:"
#read line

mkdir $dir_name
cd $dir_name

#tar dosyasını aramak için indirilenler dizinine gözat
TAR_PATH="/home/tnrdll/archive/downloads/${tar_file}"
SQL_PATH="/home/tnrdll/archive/downloads/${database_file}"




if [[ -f ${TAR_PATH}  ]]
then
    printf "${LGR}${tar_file} bulundu.${NOC}\n"

else
    printf "${RED}Belirtilen dosya bulunamadı.${NOC}\n"
    exit 1
fi

if [[ -f ${SQL_PATH}  ]]
then
    printf "${LGR}${database_file} bulundu.${NOC}\n"

else
    printf "${RED}Belirtilen dosya bulunamadı.${NOC}\n"
    exit 1
fi



echo "islemler baslatiliyor..."
sleep 2
#read -p "Press enter to continue"

cp $TAR_PATH .
cp $SQL_PATH .


FILE_NAME=$(basename -- "$database_file")
EXT="${FILE_NAME##*.}"
FILE_NAME="${FILE_NAME%.*}"


tar xvf $tar_file
gzip -d $database_file



cd var/www/
cd $(ls -d [^ABC]*/ | head -n 1) #ABC ile başlamayan dizine dal


mv * ../../..
mv .* ../../.. #move dotfiles

cd ../../..
rm -rf var
rm -rf $tar_file
rm -rf $database_file

chmod -R 777 web/sites/default/


printf "${LGR}dosyalar başarıyla yerleştirildi.${NOC}\n"

echo "db aktariliyor..."
sleep 3

#mysql -e "SHOW DATABASES";
#echo ${PWD##*/}_source

#eski source db'yi uçur.
if mysql "${PWD##*/}_source" -e exit > /dev/null 2>&1; then
    mysql -e "DROP DATABASE ${PWD##*/}_source";
fi

mysql -e "CREATE DATABASE ${PWD##*/}_source"
mysql -e "ALTER DATABASE ${PWD##*/}_source CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci"


#karakter seti hatası çıkarsa:
#sed ile sql içerisinde ara ve değiştir. s(search) g(all-global)
sed -i 's/utf8mb4_0900_ai_ci/utf8mb4_general_ci/g' $FILE_NAME

#aktar

mysql "${PWD##*/}_source" < $FILE_NAME


printf "${LGR}db aktarımı başarıyla tamamlandi.${NOC}\n"

echo "\$databases['default']['default'] = array (
  'database' => '${PWD##*/}_source',
  'username' => 'root',
  'password' => '',
  'prefix' => '',
  'host' => 'localhost',
  'port' => '3306',
  'namespace' => 'Drupal\\Core\\Database\\Driver\\mysql',
  'driver' => 'mysql',
);"  >> web/sites/default/settings.php

echo "\$config['system.logging']['error_level'] = 'verbose';" >> web/sites/default/settings.php

echo "error_reporting( E_ALL ^ ( E_NOTICE | E_WARNING | E_DEPRECATED ) );" >> web/sites/default/settings.php

echo "global \$content_directories;\$content_directories['sync'] = '../content/sync';" >> web/sites/default/settings.php


cd /var/www/html/oyas/${PWD##*/}/
mkdir content
chmod -R 777 content

drush cr