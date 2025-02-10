<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr_FR">
<context>
    <name>ConffileHandlerDialog</name>
    <message>
        <location filename="../conffilehandlerdialog.ui" line="14"/>
        <source>Configuration File Conflicts</source>
        <translation>Conflit de fichier de configuration</translation>
    </message>
    <message>
        <location filename="../conffilehandlerdialog.ui" line="24"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Some of the newly installed updates have updated configuration files. &lt;/p&gt;&lt;p&gt;Please choose what to do with these files.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Certaines mises à jour nouvellement installées ont modifié des fichiers de configuration.&lt;/p&gt;&lt;p&gt;Merci de choisir comment gérer ces fichiers.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../conffilehandlerdialog.ui" line="81"/>
        <source>Done</source>
        <translation>Terminé</translation>
    </message>
</context>
<context>
    <name>ConffileWidget</name>
    <message>
        <location filename="../conffilewidget.ui" line="14"/>
        <source>Form</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <location filename="../conffilewidget.ui" line="20"/>
        <source>Filename</source>
        <translation>Nom de fichier</translation>
    </message>
    <message>
        <location filename="../conffilewidget.ui" line="40"/>
        <source>Keep old</source>
        <translation>Conserver l’ancien</translation>
    </message>
    <message>
        <location filename="../conffilewidget.ui" line="47"/>
        <source>Replace with new</source>
        <translation>Remplacer par le nouveau</translation>
    </message>
    <message>
        <location filename="../conffilewidget.ui" line="54"/>
        <source>Show diff</source>
        <translation>Afficher les différences</translation>
    </message>
</context>
<context>
    <name>DiffDisplayDialog</name>
    <message>
        <location filename="../diffdisplaydialog.ui" line="14"/>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <location filename="../diffdisplaydialog.ui" line="24"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Lines that start with a &amp;quot;+&amp;quot; only exist in the &lt;span style=&quot; font-weight:700;&quot;&gt;new&lt;/span&gt; file.&lt;/p&gt;&lt;p&gt;Lines that start with a &amp;quot;-&amp;quot; only exist in the &lt;span style=&quot; font-weight:700;&quot;&gt;old&lt;/span&gt; file.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;
&lt;p&gt;Les lignes commençant par &amp;quot;+&amp;quot; n’existent que dans le &lt;span style=&quot;font-weight:700;&quot;&gt;nouveau&lt;/span&gt; fichier.&lt;/p&gt;
&lt;p&gt;Les lignes commençant par &amp;quot;-&amp;quot; n’existent que dans l’&lt;span style=&quot;font-weight:700;&quot;&gt;ancien&lt;/span&gt; fichier.&lt;/p&gt;
&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../diffdisplaydialog.ui" line="56"/>
        <source>Done</source>
        <translation>Terminé</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="14"/>
        <source>Lubuntu Update</source>
        <translation>Mises à jour Lubuntu</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="25"/>
        <source>0 package(s) will be updated. 0 of these updates are security-related.</source>
        <translation>0 paquets seront mis à jour. 0 de ces mises à jour sont liées à la sécurité.</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="33"/>
        <source>Packages</source>
        <translation>Paquets</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="83"/>
        <source>Install Updates</source>
        <translation>Installer les mises à jour</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="94"/>
        <source>Check for Updates</source>
        <translation>Vérifier les mises à jour</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="101"/>
        <source>Close</source>
        <translatorcomment>Standard button, should be provided and translated by Qt</translatorcomment>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="57"/>
        <source>To be installed</source>
        <translation>À installer</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="60"/>
        <source>To be upgraded</source>
        <translation>À mettre à jour</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="63"/>
        <source>To be removed</source>
        <translation>À désinstaller</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="66"/>
        <source>Held back (usually temporarily)</source>
        <translation>Conservés (généralement temporaire)</translation>
    </message>
    <message numerus="yes">
        <location filename="../mainwindow.cpp" line="80"/>
        <source>%n package(s) will be updated. </source>
        <translation>
            <numerusform>%n paquet sera mis à jour. </numerusform>
            <numerusform>%n paquets seront mis à jour. </numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location filename="../mainwindow.cpp" line="82"/>
        <source>%n of these updates are security-related.</source>
        <translation>
            <numerusform>%n de ces mises à jour est liée à la sécurité.</numerusform>
            <numerusform>%n de ces mises à jour sont liées à la sécurité.</numerusform>
        </translation>
    </message>
    <message>
        <source>%1 package(s) will be updated. %2 of these updates are security-related.</source>
        <translatorcomment>This sentence should be split and make use of https://doc.qt.io/qt-6/i18n-source-translation.html#handle-plural-forms</translatorcomment>
        <translation type="vanished">%1 paquets seront mis à jour. %2 de ces mises à jour sont liées à la sécurité.</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="149"/>
        <source>Update installation complete.</source>
        <translation>Installation des mises à jour terminée.</translation>
    </message>
</context>
<context>
    <name>Orchestrator</name>
    <message>
        <location filename="../orchestrator.cpp" line="71"/>
        <source>Updates available!

%1 to upgrade, %2 to install, and %3 to remove.

Click the tray icon to install the updates.</source>
        <translation>Mises à jour disponibles&#x202f;!

%1 à mettre à jour, %2 à installer, et %3 à désinstaller.

Cliquez sur l’icône dans la zone de notification pour installer les mises à jour.</translation>
    </message>
</context>
<context>
    <name>ReleaseUpgradeWindow</name>
    <message>
        <location filename="../releaseupgradewindow.ui" line="14"/>
        <location filename="../releaseupgradewindow.cpp" line="58"/>
        <source>Lubuntu Update</source>
        <translation>Mises à jour Lubuntu</translation>
    </message>
    <message>
        <location filename="../releaseupgradewindow.ui" line="20"/>
        <source>upgrade available</source>
        <translation>mise à jour disponible</translation>
    </message>
    <message>
        <location filename="../releaseupgradewindow.ui" line="45"/>
        <source>Upgrade Now</source>
        <translation>Mettre à jour maintenant</translation>
    </message>
    <message>
        <location filename="../releaseupgradewindow.ui" line="52"/>
        <source>Remind me later</source>
        <translation>Me rappeler plus tard</translation>
    </message>
    <message>
        <location filename="../releaseupgradewindow.ui" line="59"/>
        <source>Decline Upgrade</source>
        <translation>Ne pas mettre à jour</translation>
    </message>
    <message>
        <location filename="../releaseupgradewindow.cpp" line="14"/>
        <source>An upgrade to Lubuntu %1 is available! Would you like to install this upgrade now?</source>
        <translation>Une mise à jour vers Lubuntu %1 est disponible&#x202f;! Voulez-vous installer cette mise à jour maintenant&#x202f;?</translation>
    </message>
    <message>
        <location filename="../releaseupgradewindow.cpp" line="59"/>
        <source>You have declined the upgrade to Lubuntu %1.
You can upgrade manually by following the directions at https://manual.lubuntu.me/stable/D/upgrading.html.</source>
        <translation>Vous avez choisi de ne pas mettre à jour vers Lubuntu %1.
Vous pouvez mettre à jour manuellement en suivant les instructions sur https://manual.lubuntu.me/stable/D/upgrading.html.</translation>
    </message>
</context>
<context>
    <name>UpgradeDelayWindow</name>
    <message>
        <location filename="../upgradedelaywindow.ui" line="14"/>
        <source>Lubuntu Update</source>
        <translation>Mises à jour Lubuntu</translation>
    </message>
    <message>
        <location filename="../upgradedelaywindow.ui" line="20"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;How long would you like to wait before being reminded of the upgrade again?&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Dans combien de temps souhaitez-vous un rappel concernant la mise à jour&#x202f;?&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../upgradedelaywindow.ui" line="48"/>
        <source>OK</source>
        <translatorcomment>Standard button, should be provided and translated by Qt</translatorcomment>
        <translation>Ok</translation>
    </message>
    <message>
        <location filename="../upgradedelaywindow.ui" line="55"/>
        <source>Cancel</source>
        <translatorcomment>Standard button, should be provided and translated by Qt</translatorcomment>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../upgradedelaywindow.ui" line="63"/>
        <source>Days</source>
        <translation>Jours</translation>
    </message>
    <message>
        <location filename="../upgradedelaywindow.ui" line="68"/>
        <source>Weeks</source>
        <translation>Semaines</translation>
    </message>
    <message>
        <location filename="../upgradedelaywindow.ui" line="73"/>
        <source>Months</source>
        <translation>Mois</translation>
    </message>
</context>
</TS>
