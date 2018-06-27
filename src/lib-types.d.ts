import { Buffer } from "nbind/dist/shim";

export class NBindBase { free?(): void }

export class JSCertificate extends NBindBase {
	/** std::string sign(Buffer); */
	sign(p0: number[] | ArrayBuffer | DataView | Uint8Array | Buffer): string;

	/** std::string id; -- Read-only */
	id: string;

	/** std::string subject; -- Read-only */
	subject: string;

	/** std::string issuer; -- Read-only */
	issuer: string;
}

export class Signer extends NBindBase {
	/** static std::vector<JSCertificate> list(); */
	static list(): JSCertificate[];

	/** static JSCertificate find(std::string); */
	static find(p0: string): JSCertificate;
}
