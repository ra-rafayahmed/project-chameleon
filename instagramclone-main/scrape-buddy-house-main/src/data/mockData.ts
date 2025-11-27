import user1 from '@/assets/user1.jpg';
import user2 from '@/assets/user2.jpg';
import user3 from '@/assets/user3.jpg';
import post1 from '@/assets/post1.jpg';
import post2 from '@/assets/post2.jpg';
import post3 from '@/assets/post3.jpg';
import post4 from '@/assets/post4.jpg';
import post5 from '@/assets/post5.jpg';
import post6 from '@/assets/post6.jpg';

export interface User {
  id: string;
  username: string;
  fullName: string;
  avatar: string;
  bio: string;
  followers: number;
  following: number;
  posts: number;
  verified?: boolean;
}

export interface Comment {
  id: string;
  userId: string;
  username: string;
  text: string;
  timestamp: string;
}

export interface Post {
  id: string;
  userId: string;
  username: string;
  userAvatar: string;
  image: string;
  caption: string;
  likes: number;
  comments: Comment[];
  timestamp: string;
  location?: string;
}

export interface StoryItem {
  id: string;
  image: string;
  timestamp: string;
}

export interface Story {
  id: string;
  userId: string;
  username: string;
  avatar: string;
  items: StoryItem[];
  viewed: boolean;
}

export const users: User[] = [
  {
    id: '1',
    username: 'emma_creative',
    fullName: 'Emma Rodriguez',
    avatar: user1,
    bio: '✨ Creative soul | Travel enthusiast | Coffee addict ☕\n📍 Based in San Francisco',
    followers: 12453,
    following: 892,
    posts: 147,
    verified: true,
  },
  {
    id: '2',
    username: 'alex_photo',
    fullName: 'Alex Thompson',
    avatar: user2,
    bio: '📸 Urban photographer | Street style\n🌆 Capturing city moments\n👇 Latest work below',
    followers: 8921,
    following: 543,
    posts: 89,
  },
  {
    id: '3',
    username: 'sophie_arts',
    fullName: 'Sophie Anderson',
    avatar: user3,
    bio: '🎨 Artist & Designer | Creating magic\n🌸 Nature lover | Boho vibes\n🔗 Shop: sophiearts.com',
    followers: 15678,
    following: 1203,
    posts: 234,
    verified: true,
  },
  {
    id: '4',
    username: 'mike_fitness',
    fullName: 'Mike Johnson',
    avatar: user1,
    bio: '💪 Fitness coach | Nutrition expert\n🏋️ Transforming lives through fitness\n📧 Contact: mike@fitcoach.com',
    followers: 25340,
    following: 678,
    posts: 412,
    verified: true,
  },
  {
    id: '5',
    username: 'sarah_food',
    fullName: 'Sarah Williams',
    avatar: user2,
    bio: '🍳 Food blogger | Recipe creator\n🌮 Sharing delicious recipes daily\n📍 NYC',
    followers: 18920,
    following: 1456,
    posts: 523,
  },
  {
    id: '6',
    username: 'david_tech',
    fullName: 'David Chen',
    avatar: user3,
    bio: '💻 Software engineer | Tech enthusiast\n🚀 Building the future\n🔗 davidchen.dev',
    followers: 9543,
    following: 234,
    posts: 156,
  },
];

export const posts: Post[] = [
  {
    id: '1',
    userId: '1',
    username: 'emma_creative',
    userAvatar: user1,
    image: post1,
    caption: 'Chasing sunsets and good vibes 🌅 This moment was absolutely magical! #sunset #ocean #peaceful',
    likes: 1243,
    timestamp: '2 hours ago',
    location: 'Malibu Beach, CA',
    comments: [
      {
        id: 'c1',
        userId: '2',
        username: 'alex_photo',
        text: 'Stunning capture! The colors are incredible 🔥',
        timestamp: '1 hour ago',
      },
      {
        id: 'c2',
        userId: '3',
        username: 'sophie_arts',
        text: 'This is breathtaking! What camera did you use?',
        timestamp: '45 minutes ago',
      },
    ],
  },
  {
    id: '2',
    userId: '2',
    username: 'alex_photo',
    userAvatar: user2,
    image: post2,
    caption: 'Breakfast goals 🍳🥑 Starting the day right with this beautiful spread. Recipe coming soon!',
    likes: 892,
    timestamp: '5 hours ago',
    location: 'Brooklyn, NY',
    comments: [
      {
        id: 'c3',
        userId: '1',
        username: 'emma_creative',
        text: 'This looks amazing! Need this in my life right now 😍',
        timestamp: '4 hours ago',
      },
    ],
  },
  {
    id: '3',
    userId: '3',
    username: 'sophie_arts',
    userAvatar: user3,
    image: post3,
    caption: 'Home is where the plants are 🌿 Loving this natural light in my new space. #interiordesign #plantlover',
    likes: 2156,
    timestamp: '1 day ago',
    location: 'Portland, OR',
    comments: [
      {
        id: 'c4',
        userId: '2',
        username: 'alex_photo',
        text: 'Such a peaceful space! Love the minimalist vibe',
        timestamp: '1 day ago',
      },
      {
        id: 'c5',
        userId: '1',
        username: 'emma_creative',
        text: 'Can I move in? 😄 This is exactly my aesthetic!',
        timestamp: '23 hours ago',
      },
    ],
  },
  {
    id: '4',
    userId: '2',
    username: 'alex_photo',
    userAvatar: user2,
    image: post4,
    caption: 'City lights and late nights 🌃✨ Tokyo never disappoints. The energy here is unmatched!',
    likes: 3421,
    timestamp: '2 days ago',
    location: 'Shibuya, Tokyo',
    comments: [
      {
        id: 'c6',
        userId: '3',
        username: 'sophie_arts',
        text: 'The bokeh effect is perfect! Amazing shot 📸',
        timestamp: '2 days ago',
      },
    ],
  },
  {
    id: '5',
    userId: '1',
    username: 'emma_creative',
    userAvatar: user1,
    image: post5,
    caption: 'Meet Charlie! 🐶💛 The newest member of our family. He\'s already stolen our hearts!',
    likes: 4532,
    timestamp: '3 days ago',
    comments: [
      {
        id: 'c7',
        userId: '2',
        username: 'alex_photo',
        text: 'Oh my goodness! What a cutie! 🥺',
        timestamp: '3 days ago',
      },
      {
        id: 'c8',
        userId: '3',
        username: 'sophie_arts',
        text: 'ADORABLE! Welcome to the family, Charlie! 🐾',
        timestamp: '3 days ago',
      },
    ],
  },
  {
    id: '6',
    userId: '3',
    username: 'sophie_arts',
    userAvatar: user3,
    image: post6,
    caption: 'Adventure awaits at every turn 🏔️ Hiking through these magnificent mountains reminded me how small we are in this vast world.',
    likes: 2891,
    timestamp: '4 days ago',
    location: 'Rocky Mountains, CO',
    comments: [
      {
        id: 'c9',
        userId: '1',
        username: 'emma_creative',
        text: 'This view is absolutely incredible! Adding to my bucket list 📝',
        timestamp: '4 days ago',
      },
    ],
  },
];

export const stories: Story[] = [
  {
    id: 's1',
    userId: '1',
    username: 'emma_creative',
    avatar: user1,
    items: [
      { id: 's1-1', image: post1, timestamp: '2 hours ago' },
      { id: 's1-2', image: post5, timestamp: '3 hours ago' },
    ],
    viewed: false,
  },
  {
    id: 's2',
    userId: '2',
    username: 'alex_photo',
    avatar: user2,
    items: [
      { id: 's2-1', image: post2, timestamp: '5 hours ago' },
      { id: 's2-2', image: post4, timestamp: '6 hours ago' },
    ],
    viewed: true,
  },
  {
    id: 's3',
    userId: '3',
    username: 'sophie_arts',
    avatar: user3,
    items: [
      { id: 's3-1', image: post3, timestamp: '1 day ago' },
    ],
    viewed: false,
  },
];
