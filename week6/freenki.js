const http = require('http');
const fs = require('fs');

const encrypt = (m) => Buffer.from(m, 'utf16le').map(x => (x ^ 0x21) + 0x0f);

const decrypt = (c) => c.map(x => (x - 0x0f) ^ 0x21);

function reverseInplace (buffer) {
  for (var i = 0, j = buffer.length - 1; i < j; ++i, --j) {
    var t = buffer[j]

    buffer[j] = buffer[i]
    buffer[i] = t
  }

  return buffer
}

function xor (a, b) {
  var length = Math.max(a.length, b.length)
  var buffer = Buffer.allocUnsafe(length)

  for (var i = 0; i < length; ++i) {
    buffer[i] = a[i] ^ b[i]
  }

  return buffer
}

function xorInplace (a, b) {
  var length = Math.min(a.length, b.length)

  for (var i = 0; i < length; ++i) {
    a[i] = a[i] ^ b[i]
  }

  return a
}

const shl = (buf /*:Buffer*/, shiftBits /*:number*/) => {
    if (shiftBits < 0) { return module.exports.shr(buf, -shiftBits); }
    if (shiftBits !== (shiftBits | 0)) { throw new Error("shiftBits must be a 32 bit int"); }
    const bytes = 2 * ((shiftBits >> 4) + ((shiftBits & 15) !== 0));
    const bits = (bytes * 8) - shiftBits;
    for (let reg = 0, i = 0; i - bytes < buf.length; i += 2) {
        reg <<= 16;
        if (i < buf.length - 1) {
            reg |= buf.readUInt16BE(i);
        } else if (i < buf.length) {
            reg |= buf[i] << 8;
        }
        if (i - bytes >= 0) {
            if (i - bytes < buf.length - 1) {
                buf.writeUInt16BE((reg >>> bits) & 0xffff, i - bytes);
            } else {
                if (i - bytes !== buf.length - 1) { throw new Error(); }
                buf[i - bytes] = reg >>> (bits + 8);
            }
        } else if (i - bytes === -1) {
            buf[0] = reg >>> bits;
        }
    }
};

const shr = (buf /*:Buffer*/, shiftBits /*:number*/) => {
    if (shiftBits < 0) { return shl(buf, -shiftBits); }
    if (shiftBits !== (shiftBits | 0)) { throw new Error("shiftBits must be a 32 bit int"); }
    const bytes = 2 * ((shiftBits >> 4) + ((shiftBits & 15) !== 0));
    const bits = 16 - ((bytes * 8) - shiftBits);
    for (let reg = 0, i = buf.length - 2; i + bytes >= -1; i -= 2) {
        reg >>>= 16;
        if (i >= 0) {
            reg |= buf.readUInt16BE(i) << 16;
        } else if (i === -1) {
            reg |= buf[0] << 16;
        }
        if (i + bytes + 1 < buf.length) {
            if (i + bytes >= 0) {
                buf.writeUInt16BE((reg >>> bits) & 0xffff, i + bytes);
            } else {
                if (i + bytes + 1) { throw new Error(); }
                buf[0] = reg >>> bits;
            }
        } else if (i + bytes + 1 === buf.length) {
            buf[i + bytes] = reg >>> (bits + 8);
        }
    }
};

const parity = (m) => {
  m = Buffer.from(m);
  let xmm0 = Buffer.from('00000000000000000000000000000000', 'hex');
  let xmm1 = Buffer.from('00000000000000000000000000000000', 'hex');
  let xmm2 = Buffer.from('00000000000000000000000000000000', 'hex');
  let i;
  for (i = 0; i < Math.floor(m.length / 0x20) * 0x20; i += 0x20) {
    xorInplace(xmm2, m.slice(i, i+0x10));
    xorInplace(xmm1, m.slice(i+0x10, i+2*0x10));
  }

  xorInplace(xmm1, xmm2);
  reverseInplace(xmm0);
  reverseInplace(xmm1);

  xmm1.copy(xmm0);
  shr(xmm0, 8 * 8);
  xorInplace(xmm1, xmm0);

  xmm1.copy(xmm0);
  shr(xmm0, 4 * 8);
  xorInplace(xmm1, xmm0);

  xmm1.copy(xmm0);
  shr(xmm0, 2 * 8);
  xorInplace(xmm1, xmm0);

  xmm1.copy(xmm0);
  shr(xmm0, 1 * 8);
  xorInplace(xmm1, xmm0);

  parity_bit = xmm1[15];
  for (; i < m.length; i++)
    parity_bit ^= m[i];

  // console.log(m);
  console.log(parity_bit.toString(16))
  return Buffer.from(parity_bit.toString(16), 'hex');
}


const hacked_binary = Buffer.from(fs.readFileSync('./freenki_hacked.exe', { encoding: 'hex' }), 'hex');

console.log(hacked_binary);
console.log(encrypt(hacked_binary));
console.log(Buffer.from(decrypt(encrypt(hacked_binary))));

const requestHandler = (request, response) => {

  console.log('================INCOMMING REQUEST===============');
  console.log(request.method + ":" + request.url);
  console.log(request.headers);

  if (request.url == '/hello_world') {
    payload = encrypt(hacked_binary);
    msg = Buffer.concat([Buffer.from('PNGF'), parity(payload), payload]);
    console.log('=> ' + msg);
    response.write(msg);
    response.end();
    return;
  }

  let body = [];
  request.on('error', (err) => {
    console.error(err);
  }).on('data', (chunk) => {
    body.push(chunk);
  }).on('end', () => {
    body = Buffer.concat(body)
    data = body.slice(1+2*6);

    body = body.toString();
    // At this point, we have the headers, method, url and body, and can now
    // do whatever we need to in order to respond to this request.
    code = body[0];
    mac = body.slice(1, 2*6);

    console.log(code + " " + mac);
    if (code !== '4') console.log(decrypt(data).toString());

    response.statusCode = 200;
    let msg;
    if (code == '0')
      msg = 'Freenki response\0';
    else if (code == '2')
      msg = encrypt('1http://old.jrchina.com/hello_world\r\n\0');
    else
      msg = encrypt('dummy\0');
    console.log('=> ' + msg);
    response.write(msg);
    response.end();
  });
}

const port = 3000;
const server = http.createServer(requestHandler)
server.listen(port, (err) => {
  if (err) {
    return console.log('something bad happened', err)
  }

  console.log(`server is listening on ${port}`)
});
