00016a0f <__i686.get_pc_thunk.bx>:
   16a0f:	8b 1c 24             	mov    ebx,DWORD PTR [esp]
   16a12:	c3                   	ret    

0002d2c0 <getenv>:
   2d2c0:	55                   	push   ebp
   2d2c1:	89 e5                	mov    ebp,esp
   2d2c3:	83 ec 24             	sub    esp,0x24
   2d2c6:	89 5d f4             	mov    DWORD PTR [ebp-0xc],ebx          ; local backup of ebx ("push ebx")
   2d2c9:	e8 41 97 fe ff       	call   16a0f <__i686.get_pc_thunk.bx>   ; get eip (destroy ebx version)
   2d2ce:	81 c3 26 0d 13 00    	add    ebx,0x130d26                     ; relocation: add offset into .so
   2d2d4:	89 75 f8             	mov    DWORD PTR [ebp-0x8],esi          ; local backup of esi ("push esi")
   2d2d7:	8b 75 08             	mov    esi,DWORD PTR [ebp+0x8]          ; param into esi
   2d2da:	89 7d fc             	mov    DWORD PTR [ebp-0x4],edi          ; local backup of edi ("push edi")
   2d2dd:	89 34 24             	mov    DWORD PTR [esp],esi              ; param on stack
   2d2e0:	e8 7b 8b 04 00       	call   75e60 <strlen>
   2d2e5:	89 c2                	mov    edx,eax                          ; edx = strlen 
   2d2e7:	8b 83 a8 ff ff ff    	mov    eax,DWORD PTR [ebx-0x58]         ; eax = **environ
   2d2ed:	8b 38                	mov    edi,DWORD PTR [eax]              ; 
   2d2ef:	85 ff                	test   edi,edi
   2d2f1:	75 15                	jne    2d308 <getenv+0x48>
   2d2f3:	31 c0                	xor    eax,eax
   2d2f5:	8b 5d f4             	mov    ebx,DWORD PTR [ebp-0xc]
   2d2f8:	8b 75 f8             	mov    esi,DWORD PTR [ebp-0x8]
   2d2fb:	8b 7d fc             	mov    edi,DWORD PTR [ebp-0x4]
   2d2fe:	89 ec                	mov    esp,ebp
   2d300:	5d                   	pop    ebp
   2d301:	c3                   	ret    
   2d302:	8d b6 00 00 00 00    	lea    esi,[esi+0x0]
   2d308:	80 3e 00             	cmp    BYTE PTR [esi],0x0
   2d30b:	74 e6                	je     2d2f3 <getenv+0x33>
   2d30d:	80 7e 01 00          	cmp    BYTE PTR [esi+0x1],0x0
   2d311:	75 35                	jne    2d348 <getenv+0x88>
   2d313:	8b 07                	mov    eax,DWORD PTR [edi]
   2d315:	0f b6 16             	movzx  edx,BYTE PTR [esi]
   2d318:	85 c0                	test   eax,eax
   2d31a:	74 d7                	je     2d2f3 <getenv+0x33>
   2d31c:	0f b6 d2             	movzx  edx,dl
   2d31f:	80 ce 3d             	or     dh,0x3d
   2d322:	66 3b 10             	cmp    dx,WORD PTR [eax]
   2d325:	75 12                	jne    2d339 <getenv+0x79>
   2d327:	e9 8c 00 00 00       	jmp    2d3b8 <getenv+0xf8>
   2d32c:	8d 74 26 00          	lea    esi,[esi+eiz*1+0x0]
   2d330:	66 3b 10             	cmp    dx,WORD PTR [eax]
   2d333:	0f 84 7f 00 00 00    	je     2d3b8 <getenv+0xf8>
   2d339:	83 c7 04             	add    edi,0x4
   2d33c:	8b 07                	mov    eax,DWORD PTR [edi]
   2d33e:	85 c0                	test   eax,eax
   2d340:	75 ee                	jne    2d330 <getenv+0x70>
   2d342:	eb af                	jmp    2d2f3 <getenv+0x33>
   2d344:	8d 74 26 00          	lea    esi,[esi+eiz*1+0x0]
   2d348:	0f b7 06             	movzx  eax,WORD PTR [esi]
   2d34b:	83 ea 02             	sub    edx,0x2
   2d34e:	83 c6 02             	add    esi,0x2
   2d351:	89 55 ec             	mov    DWORD PTR [ebp-0x14],edx
   2d354:	89 75 f0             	mov    DWORD PTR [ebp-0x10],esi
   2d357:	66 89 45 ea          	mov    WORD PTR [ebp-0x16],ax
   2d35b:	8b 37                	mov    esi,DWORD PTR [edi]
   2d35d:	85 f6                	test   esi,esi
   2d35f:	75 3c                	jne    2d39d <getenv+0xdd>
   2d361:	eb 90                	jmp    2d2f3 <getenv+0x33>
   2d363:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
   2d366:	8b 55 f0             	mov    edx,DWORD PTR [ebp-0x10]
   2d369:	89 44 24 08          	mov    DWORD PTR [esp+0x8],eax
   2d36d:	8d 46 02             	lea    eax,[esi+0x2]
   2d370:	89 54 24 04          	mov    DWORD PTR [esp+0x4],edx
   2d374:	89 04 24             	mov    DWORD PTR [esp],eax
   2d377:	e8 04 8d 04 00       	call   76080 <strncmp>
   2d37c:	85 c0                	test   eax,eax
   2d37e:	75 10                	jne    2d390 <getenv+0xd0>
   2d380:	8b 45 ec             	mov    eax,DWORD PTR [ebp-0x14]
   2d383:	80 7c 06 02 3d       	cmp    BYTE PTR [esi+eax*1+0x2],0x3d
   2d388:	74 1e                	je     2d3a8 <getenv+0xe8>
   2d38a:	8d b6 00 00 00 00    	lea    esi,[esi+0x0]
   2d390:	83 c7 04             	add    edi,0x4
   2d393:	8b 37                	mov    esi,DWORD PTR [edi]
   2d395:	85 f6                	test   esi,esi
   2d397:	0f 84 56 ff ff ff    	je     2d2f3 <getenv+0x33>
   2d39d:	0f b7 55 ea          	movzx  edx,WORD PTR [ebp-0x16]
   2d3a1:	66 3b 16             	cmp    dx,WORD PTR [esi]
   2d3a4:	75 ea                	jne    2d390 <getenv+0xd0>
   2d3a6:	eb bb                	jmp    2d363 <getenv+0xa3>
   2d3a8:	8d 44 06 03          	lea    eax,[esi+eax*1+0x3]
   2d3ac:	8d 74 26 00          	lea    esi,[esi+eiz*1+0x0]
   2d3b0:	e9 40 ff ff ff       	jmp    2d2f5 <getenv+0x35>
   2d3b5:	8d 76 00             	lea    esi,[esi+0x0]
   2d3b8:	83 c0 02             	add    eax,0x2
   2d3bb:	90                   	nop
   2d3bc:	8d 74 26 00          	lea    esi,[esi+eiz*1+0x0]
   2d3c0:	e9 30 ff ff ff       	jmp    2d2f5 <getenv+0x35>



00075e60 <strlen>:
   75e60:	8b 44 24 04          	mov    eax,DWORD PTR [esp+0x4]
   75e64:	ba 03 00 00 00       	mov    edx,0x3
   75e69:	21 c2                	and    edx,eax
   75e6b:	74 24                	je     75e91 <strlen+0x31>
   75e6d:	7a 17                	jp     75e86 <strlen+0x26>
   75e6f:	38 30                	cmp    BYTE PTR [eax],dh
   75e71:	0f 84 9f 00 00 00    	je     75f16 <strlen+0xb6>
   75e77:	40                   	inc    eax
   75e78:	38 30                	cmp    BYTE PTR [eax],dh
   75e7a:	0f 84 96 00 00 00    	je     75f16 <strlen+0xb6>
   75e80:	40                   	inc    eax
   75e81:	83 f2 02             	xor    edx,0x2
   75e84:	74 0b                	je     75e91 <strlen+0x31>
   75e86:	38 30                	cmp    BYTE PTR [eax],dh
   75e88:	0f 84 88 00 00 00    	je     75f16 <strlen+0xb6>
   75e8e:	40                   	inc    eax
   75e8f:	31 d2                	xor    edx,edx
   75e91:	8b 08                	mov    ecx,DWORD PTR [eax]
   75e93:	83 c0 04             	add    eax,0x4
   75e96:	29 ca                	sub    edx,ecx
   75e98:	81 c1 ff fe fe fe    	add    ecx,0xfefefeff
   75e9e:	4a                   	dec    edx
   75e9f:	73 58                	jae    75ef9 <strlen+0x99>
   75ea1:	31 ca                	xor    edx,ecx
   75ea3:	81 e2 00 01 01 01    	and    edx,0x1010100
   75ea9:	75 4e                	jne    75ef9 <strlen+0x99>
   75eab:	8b 08                	mov    ecx,DWORD PTR [eax]
   75ead:	83 c0 04             	add    eax,0x4
   75eb0:	29 ca                	sub    edx,ecx
   75eb2:	81 c1 ff fe fe fe    	add    ecx,0xfefefeff
   75eb8:	4a                   	dec    edx
   75eb9:	73 3e                	jae    75ef9 <strlen+0x99>
   75ebb:	31 ca                	xor    edx,ecx
   75ebd:	81 e2 00 01 01 01    	and    edx,0x1010100
   75ec3:	75 34                	jne    75ef9 <strlen+0x99>
   75ec5:	8b 08                	mov    ecx,DWORD PTR [eax]
   75ec7:	83 c0 04             	add    eax,0x4
   75eca:	29 ca                	sub    edx,ecx
   75ecc:	81 c1 ff fe fe fe    	add    ecx,0xfefefeff
   75ed2:	4a                   	dec    edx
   75ed3:	73 24                	jae    75ef9 <strlen+0x99>
   75ed5:	31 ca                	xor    edx,ecx
   75ed7:	81 e2 00 01 01 01    	and    edx,0x1010100
   75edd:	75 1a                	jne    75ef9 <strlen+0x99>
   75edf:	8b 08                	mov    ecx,DWORD PTR [eax]
   75ee1:	83 c0 04             	add    eax,0x4
   75ee4:	29 ca                	sub    edx,ecx
   75ee6:	81 c1 ff fe fe fe    	add    ecx,0xfefefeff
   75eec:	4a                   	dec    edx
   75eed:	73 0a                	jae    75ef9 <strlen+0x99>
   75eef:	31 ca                	xor    edx,ecx
   75ef1:	81 e2 00 01 01 01    	and    edx,0x1010100
   75ef7:	74 98                	je     75e91 <strlen+0x31>
   75ef9:	83 e8 04             	sub    eax,0x4
   75efc:	81 e9 ff fe fe fe    	sub    ecx,0xfefefeff
   75f02:	80 f9 00             	cmp    cl,0x0
   75f05:	74 0f                	je     75f16 <strlen+0xb6>
   75f07:	40                   	inc    eax
   75f08:	84 ed                	test   ch,ch
   75f0a:	74 0a                	je     75f16 <strlen+0xb6>
   75f0c:	c1 e9 10             	shr    ecx,0x10
   75f0f:	40                   	inc    eax
   75f10:	80 f9 00             	cmp    cl,0x0
   75f13:	74 01                	je     75f16 <strlen+0xb6>
   75f15:	40                   	inc    eax
   75f16:	2b 44 24 04          	sub    eax,DWORD PTR [esp+0x4]
   75f1a:	c3                   	ret    
