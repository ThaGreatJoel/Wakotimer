import dbConnect from '../../../lib/db';
import mongoose from 'mongoose';

const UserSchema = new mongoose.Schema({
  email: String,
  hackaTimeApiKey: String,
});
const User = mongoose.models.User || mongoose.model('User', UserSchema);

export default async function handler(req, res) {
  if (req.method !== 'POST') return res.status(405).end();

  await dbConnect();

  const { email, hackaTimeApiKey } = req.body;
  let user = await User.findOne({ email });
  if (!user) user = await User.create({ email, hackaTimeApiKey });

  res.status(200).json({ message: 'Logged in', userId: user._id });
}
