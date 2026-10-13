# ONLYOFFICE WPS + MIDIA Cloud 9.4.0.2

Mac Apple Silicon / Windows x64. Importa fotos WPS con el mismo conversor nativo validado en 9.4.0.1.

En Office, entra en Conectar a cloud → MIDIA Cloud e inicia sesión con tu usuario de MERP. Se aplican la misma red, MFA y sesión que en MERP. Tras conectar este equipo, los archivos locales que abras se suben a tu cuenta privada y los cambios guardados se sincronizan cada cinco segundos. El editor guarda automáticamente cada diez segundos. No se sube el historial de documentos anterior a la instalación.

Mis archivos y Compartidos conmigo se actualizan cada quince segundos. Comparte con un usuario de MERP y elige lectura o edición. El propietario puede retirar el permiso; la copia ya descargada no puede borrarse del equipo del destinatario. Compartir no envía correos ni mensajes externos.

Sin conexión, conserva y sigue editando la copia local; se vuelve a intentar la subida. Si otra persona guarda antes, no se sobrescribe su versión: guarda tu copia con otro nombre desde Office para conservar ambas. No incluye coedición simultánea en tiempo real. Cada archivo admite 25 MB, cada cuenta 500 MB y se conservan hasta diez versiones.

No publiques contraseñas ni tokens. El agente solo escucha en 127.0.0.1:43129 y admite el portal HTTPS de MIDIA. La sesión queda en el perfil privado del usuario del sistema. Cerrar sesión desde MIDIA Cloud detiene el guardado de esa cuenta en el equipo. Si compartís una cuenta del sistema operativo, las copias descargadas siguen en su disco; usad cuentas separadas para documentos privados.

Mac: cierra ONLYOFFICE WPS y reemplaza la aplicación con la del DMG. Abre la nueva aplicación para iniciar el agente. Firma local sin notarización Apple.
Windows: ejecuta el instalador; instala por usuario sin administrador. Elige ONLYOFFICE WPS como aplicación predeterminada si Windows lo pide. El agente arranca con la aplicación y al iniciar sesión en Windows. Instalador sin firma comercial.

Fuente de integración: https://github.com/guozz1984/midia-erp (app-office-cloud, bridge-v4/office/office-cloud.js).
Fuente del editor/conversor AGPL-3.0: https://github.com/guozz1984/core.
Conserva los avisos y licencias de ONLYOFFICE incluidos en la aplicación.
