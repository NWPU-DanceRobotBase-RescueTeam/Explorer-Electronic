glvs
fid = fopen('11.txt', 'r');
[dd, mn] = fread(fid, inf, 'uchar'); dd(3:3:end) = [];  % ���ļ�ȥ�ո�
fclose(fid);
sz = fix(length(dd)/8);
dd1 = reshape(dd(1:sz*8),8,sz)'; dd1 = char(dd1);  % ת��Ϊ16�����ַ���ʽ
dd2 = typecast(uint32(hex2dec(dd1(:,[7,8,5,6,3,4,1,2]))),'single');  % ת��Ϊ�����ȸ���
sz = fix(length(dd2)/35);
dd2 = reshape(dd2(1:35*sz),35,sz);  dd2 = dd2';  % ��34�з���
imu = [[dd2(:,3:5)*glv.dps,dd2(:,6:8)]*0.01,dd2(:,2)]; imuplot(imu,1);
return

figure, plot(fix(dd2(:,23)),(dd2(:,23)-fix(dd2(:,23)))*100)