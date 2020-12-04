# Laboratory work №3.
## Program's options:
<ul>
    <li><code>-c</code> - use the program to encrypt a stream or a file. It's a default option;</li>
    <li><code>-d</code> - use the program to decrypt a stream or a file;</li>
    <li><code>-h</code> - use the program to calculate hash of a file using AES128;</li>
    <li><code>-a</code> - choose an algorithm for encryption/decryption:
        <ul>
            <li><code>rc4</code> - use RC4. It's a default algorithm;</li>
            <li><code>aes128</code> - use AES128;</li>
            <li><code>aes192</code> - use AES192;</li>
            <li><code>aes256</code> - use AES256;</li>
        </ul>
    </li>
    <li><code>-f</code> - path of the file to encrypt using AES or calculate hash.</li>
</ul>

## Example:
### RC4 encryption of a stream:
<code>./laboratory_work_3 -c -a rc4</code><br>
<code><strong>Enter a key length(in bytes):</strong></code><br>
<code>16</code><br>
<code><strong>Enter key values in bytes. Every value is a digit between 0x00 and 0xFF:</strong></code><br>
<code>0F 15 71 C9 47 D9 E8 59 0C B7 AD DF AF 7F 67 98</code><br>
<code><strong>Enter a text string. Enter Ctrl+D to stop. The coded text will be displayed as the
digits between 0(0x00) and 255(0xFF) for every string:</strong></code><br>
<code>hi, how are you?</code><br>
<code><strong>16 10 251 237 188 174 79 204 129 15 118 184 36 103 121 40</strong></code><br>
<code>i'm fine, and you?</code><br>
<code><strong>247 243 197 228 74 135 110 85 210 159 240 194 155 208 81 109 119 130</strong></code>

### RC4 decryption of a stream:
<code>./laboratory_work_3 -d -a rc4</code><br>
<code><strong>Enter a key length(in bytes):</strong></code><br>
<code>16</code><br>
<code><strong>Enter key values in bytes. Every value is a digit between 0x00 and 0xFF:</strong></code><br>
<code>0F 15 71 C9 47 D9 E8 59 0C B7 AD DF AF 7F 67 98</code><br>
<code><strong>Enter a coded text string as a sequence of the digits between 0(0x00) and 255(0xFF).
Enter -1 to get current result. Enter -2 to stop:</strong></code><br>
<code>16 10 251 237 188 174 79 204 129 15 118 184 36 103 121 40</code><br>
<code>-1</code><br>
<code><strong>hi, how are you?</strong></code><br>
<code>247 243 197 228 74 135 110 85 210 159 240 194 155 208 81 109 119 130</code><br>
<code>-1</code><br>
<code><strong>i'm fine, and you?</strong></code><br>
<code>-2</code>

### AES128/192/256 encryption of a file(example for AES128):
<code>./laboratory_work_3 -c -a aes128 -f Lab3.docx</code><br>
<code><strong>Enter key values in bytes. Every value is a digit between 0x00 and 0xFF:</strong></code><br>
<code>0F 15 71 C9 47 D9 E8 59 0C B7 AD DF AF 7F 67 98</code>

### AES128/192/256 decryption of a file(example for AES128):
<code>./laboratory_work_3 -d -a aes128 -f Lab3.docx.out</code><br>
<code><strong>Enter key values in bytes. Every value is a digit between 0x00 and 0xFF:</strong></code><br>
<code>0F 15 71 C9 47 D9 E8 59 0C B7 AD DF AF 7F 67 98</code>

### Calculate hash of a file:
<code>./laboratory_work_3 -h -f Lab3.docx</code><br>
<code><strong>128 197 241 151 115 187 182 99 181 36 253 131 146 51 235 102</strong></code>
